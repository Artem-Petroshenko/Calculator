#include "Operators.h"

Operators::Operators() {
	operations["+"] = [](double a, double b) { return b + a; };
	operations["-"] = [](double a, double b) { return b - a; };
	operations["*"] = [](double a, double b) { return b * a; };
	operations["/"] = [](double a, double b) { return b / a; };
	operations["~"] = [](double a, double b = 0) { return -a; };
	operations["^"] = [](double a, double b) { return pow(b, a); };

	priorities["+"] = 1;
	priorities["-"] = 1;
	priorities["*"] = 2;
	priorities["/"] = 2;
	priorities["~"] = 3;
	priorities["^"] = 4;
	priorities["("] = 0;
}

Operators& Operators::GetOperators() {
	static Operators object;
	return object;
}

int Operators::priority(std::string const& name) {
	return priorities[name];
}

double Operators::operation(double const& a, double const& b, std::string name) {
	return operations[name](a, b);
}