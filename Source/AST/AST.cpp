#include "AST.h"
#include <string>

AstValue *Literal::value() {
	if (!value_) {
		const auto &rawString = token_.value;
		switch (token_.type) {
		case Token::INT:
			value_ = new AstValue(std::stoi(rawString));

		case Token::REAL:
			value_ = new AstValue(std::stof(rawString));

		case Token::STRING_LITERAL:
			value_ = new AstValue(rawString);
		}
	}
	return value_;
}

Variable *VariableProxy::variable() {
	if (!variable_) {
		const auto &name = token_.value;
		variable_ = new Variable(name);
	}
	return variable_;
}
