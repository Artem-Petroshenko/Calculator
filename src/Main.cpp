#include "Calc.h"

int main() {
	Calculator calculator;
	std::string expression;
	while (true) {
		std::cout << "Enter your expression or type 'exit' to end this session" << std::endl;
		std::getline(std::cin, expression);
		if (expression == "" || expression == "exit")
			break;
		try {
			std::optional<double> result = calculator.execute(expression);
			if (!result.has_value())
				std::cout << "bbb" << std::endl;
			else
				std::cout << "= " << result.value() << std::endl << std::endl;
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}
		calculator.clear();
	}
	return 0;
}