#include "AST.h"
#include <string>

Variable VariableProxy::value() const {
	const auto &rawString = token_.value;
	switch (token_.type) {
	case Token::INT:
		return Variable(std::stoi(rawString));

	case Token::REAL:
		return Variable(std::stof(rawString));

	case Token::STRING_LITERAL:
	default:
		return Variable(rawString);
	}
}
