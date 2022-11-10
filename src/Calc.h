#pragma once
#include <stack>
#include <vector>
#include <optional>

#include "Operators.h"
#include "PluginLoader.h"

using token_vector = std::vector<Token>;

class Calculator {
public:
	Calculator() = default;

	std::optional<double> execute(std::string& expression);
	void clear();

private:
	PluginLoader loader;

	token_vector tokens;
	token_vector tokens_RPN;

	std::stack<double> numbers;
	std::stack<Token> operations;

	void splitExpr(std::string& expression);
	void checkExpression();
	void getRPN();
	void executeOperation(Token& t);
	void executeFunction(Token& t);
	std::optional<double> calculate();
};