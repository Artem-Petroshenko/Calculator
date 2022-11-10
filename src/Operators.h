#pragma once
#include <map>
#include <functional>

#include "Token.h"

class Operators {
public:
	static Operators& GetOperators();
	int priority(std::string const& name);
	double operation(double const& a, double const& b, std::string name);

private:
	Operators();
	Operators(const Operators&) = delete;
	Operators& operator=(const Operators&) = delete;

	std::map<std::string, std::function<double(double, double)>> operations;
	std::map<std::string const, int> priorities;
};