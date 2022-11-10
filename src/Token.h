#pragma once
#include <iostream>
#include <string>

enum class TokenType {
	number,
	un_operation,
	bin_operation,
	prefix_function,
	postfix_function,
	openBracket,
	closeBracket,
	undefined
};

class Token {
public:
	Token() = default;

	bool operator ==(Token const& t) { return (str == t.str && type == t.type); };
	bool operator !=(Token const& t) { return (str != t.str && type != t.type); };

	std::string str = "";
	TokenType type = TokenType::undefined;
};
