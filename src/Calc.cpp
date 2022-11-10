#include "Calc.h"

void removeSpaces(std::string& expression) {
	while (expression.find(' ') != std::string::npos) {
		expression.erase(expression.find(' '), 1);
	}
}

bool isOperator(char const& c) {
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '^');
}

bool isDigit(char const& c) {
	return ((c >= '0' && c <= '9') || c == '.');
}

bool isLetter(char const& c) {
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '!');
}

void Calculator::splitExpr(std::string& expression) {
	removeSpaces(expression); //numbers are always diveded by operations
	Token token;
	for (int i = 0; i < expression.size(); i++) {
		token.str = "";
		token.type = TokenType::undefined;
		if (isOperator(expression[i])) {
			token.str = expression[i];
			if ((expression[i] == '-') &&
				(i == 0 || (i != 0 && expression[i - 1] != ')' && isOperator(expression[i - 1])))) {
				token.str = '~';
				token.type = TokenType::un_operation;
			}
			else if (expression[i] == '(')
				token.type = TokenType::openBracket;
			else if (expression[i] == ')')
				token.type = TokenType::closeBracket;
			else
				token.type = TokenType::bin_operation;
			tokens.push_back(token);
		}
		else if (isDigit(expression[i])) {
			while (isDigit(expression[i])) {
				token.str += expression[i];
				i++;
			}
			token.type = TokenType::number;
			tokens.push_back(token);
			i--;
		}
		else if (isLetter(expression[i])) {
			while (isLetter(expression[i])) {
				token.str += tolower(expression[i]);
				i++;
			}
			if (token.str == "!")
				token.str = "fact";
			if (!tokens.empty() && tokens.back().type == TokenType::number)
				token.type = TokenType::postfix_function;
			else
				token.type = TokenType::prefix_function;
			tokens.push_back(token);
			i--;
		}
	}
	return;
}

void Calculator::checkExpression() {
	//checking brackets
	std::stack<Token> brackets;
	for (auto& t : tokens) {
		if (t.str == "(")
			brackets.push(t);
		else if (t.str == ")") {
			if (brackets.empty()) {
				throw std::exception("Missing '('");
				return;
			}
			brackets.pop();
		}
	}
	if (!brackets.empty()) {
		throw std::exception("Missing ')'");
		return;
	}

	//checking operations
	char op_row_flag = 0;
	for (auto& t : tokens) {
		if (t == tokens[0] && t.type == TokenType::bin_operation) {
			throw std::exception("Expression can't start with an binary operation");
			return;
		}
		else if (t == tokens[tokens.size() - 1] && t.type == TokenType::bin_operation) {
			throw std::exception("Expression can't end with an binary operation");
			return;
		}
		else if (t != tokens[tokens.size() - 1] && (t.type == TokenType::bin_operation && op_row_flag == 1)) {
			throw std::exception("2 binary operations in a row");
			return;
		}
		else if (t.type == TokenType::bin_operation) {
			op_row_flag++;
			continue;
		}
		op_row_flag = 0;
	}
	return;
}

void Calculator::getRPN() {
	for (auto& t : tokens) {
		if (t.type == TokenType::number || t.type == TokenType::postfix_function)
			tokens_RPN.push_back(t);
		else if (t.type == TokenType::un_operation || t.type == TokenType::bin_operation) {
			if (!operations.empty())
				while (!operations.empty() && (Operators::GetOperators().priority(operations.top().str) >=
					Operators::GetOperators().priority(t.str) || operations.top().type == TokenType::prefix_function)) {
					tokens_RPN.push_back(operations.top());
					operations.pop();
				}
			operations.push(t);
		}
		else if (t.type == TokenType::openBracket || t.type == TokenType::prefix_function)
			operations.push(t);
		else if (t.type == TokenType::closeBracket) {
			while (operations.top().type != TokenType::openBracket) {
				tokens_RPN.push_back(operations.top());
				operations.pop();
			}
			operations.pop();
		}
	}
	while (!operations.empty()) {
		tokens_RPN.push_back(operations.top());
		operations.pop();
	}
	return;
}

void Calculator::executeOperation(Token& t) {
	double a = numbers.top();
	numbers.pop();
	double b = 0.0;
	if (t.type == TokenType::bin_operation) {
		b = numbers.top();
		numbers.pop();
	}
	else if (t.type != TokenType::un_operation) {
		throw std::exception("Error in tokenization");
		return;
	}
	double result = Operators::GetOperators().operation(a, b, t.str);
	numbers.push(result);
	return;
}

void Calculator::executeFunction(Token& t) {
	double a = numbers.top();
	numbers.pop();

	if (!loader.getFunction().contains("My" + t.str)) {
		throw std::exception("No such function");
		return;
	}
	double res = loader.executeFunction(a, "My" + t.str);
	numbers.push(res);
}

std::optional<double> Calculator::calculate() {
	for (auto& t : tokens_RPN) {
		if (t.type == TokenType::number)
			numbers.push(stod(t.str));
		else if (t.type == TokenType::un_operation || t.type == TokenType::bin_operation) {
			try {
				executeOperation(t);
			}
			catch (std::exception& e) {
				std::cout << e.what() << std::endl;
				return std::nullopt;
			}
		}
		else if (t.type == TokenType::prefix_function || t.type == TokenType::postfix_function) {
			try {
				executeFunction(t);
			}
			catch (std::exception& e) {
				std::cout << e.what() << std::endl;
				return std::nullopt;
			}
		}
	}
	return numbers.top();
}

std::optional<double> Calculator::execute(std::string& expression) {
	try {
		splitExpr(expression);
		checkExpression();
		getRPN();
		return calculate();
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return std::nullopt;
	}
}

void Calculator::clear() {
	tokens.clear();
	tokens_RPN.clear();
	while (!numbers.empty())
		numbers.pop();
	while (!operations.empty())
		operations.pop();
}
