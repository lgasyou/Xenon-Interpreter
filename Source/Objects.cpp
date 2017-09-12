#include "Objects.h"
#include "AST/AstValue.h"
#include "AST/AST.h"
#include "AST/Variable.h"
#include "AST/Scope.h"

IntegerObject *Object::AsInteger() {
	return static_cast<IntegerObject *>(this);
}

RealObject *Object::AsReal() {
	return static_cast<RealObject *>(this);
}

StringObject *Object::AsString() {
	return static_cast<StringObject *>(this);
}

FunctionObject *Object::AsFunction() {
	return static_cast<FunctionObject *>(this);
}

Block *FunctionObject::setup(const std::vector<AstValue> &formalArguments) {
	auto matchedFunction = getMatchedFunction(formalArguments);
	auto scope = matchedFunction.block->scope();
	for (int i = 0; i != formalArguments.size(); ++i) {
		auto fa = formalArguments[i];
		auto aa = matchedFunction.actual_arguments[i];
		scope->declarateVariable(aa);
		auto variableName = aa->variableProxy()->variable()->name();
		auto var = scope->lookup(variableName, /* createIfNotFound */true);
		*var = fa;
	}
	return matchedFunction.block;
}

void FunctionObject::addOverloadedFunction(Block *block, const std::vector<VariableDeclaration*> &actualArguments) {
	OverloadedFunction func = { actualArguments, block };
	overloaded_functions_.push_back(func);
}

AstValue::Type getFormalArgumentType(const AstValue &av) {
	return av.type();
}

Token::Type getActualArgumentType(VariableDeclaration *vd) {
	return vd->variableType();
}

bool isFormalArgTypeSameAsActualArgType(const AstValue &av, VariableDeclaration *vd) {
	return getFormalArgumentType(av) == getActualArgumentType(vd);
}

FunctionObject::OverloadedFunction &FunctionObject::getMatchedFunction(const std::vector<AstValue> &formalArguments) {
	for (auto &of : overloaded_functions_) {
		const auto &aa = of.actual_arguments;
		const auto &fa = formalArguments;
		bool matched = true;
		if (aa.size() == fa.size()) {
			for (int i = 0; i != fa.size(); ++i) {
				if (!isFormalArgTypeSameAsActualArgType(fa[i], aa[i])) {
					matched = false;
					break;
				}
			}
			if (matched) {
				return of;
			}
		}
	}
	UNREACHABLE();
}

AstValue Object::operator=(const AstValue &rhs) {
	switch (type()) {
	case Object::INTEGER:
		AsInteger()->setValue(rhs.toInt());
		break;

	case Object::REAL:
		AsReal()->setValue(rhs.toReal());
		break;

	case Object::STRING:
		AsString()->setValue(rhs.toString());
		break;
	}
	return rhs;
}

Object *Object::FromAstValue(const AstValue &rhs) {
	switch (rhs.type()) {
	case AstValue::INTEGER:
		return new IntegerObject(rhs.toInt());

	case AstValue::REAL:
		return new RealObject(rhs.toReal());

	case AstValue::STRING:
		return new StringObject(rhs.toString());

	default:
		UNREACHABLE();
	}
}

AstValue Object::toAstValue() {
	switch (type()) {
	case Object::INTEGER:
		return AstValue(AsInteger()->value());

	case Object::REAL:
		return AstValue(AsReal()->value());

	case Object::STRING:
		return AstValue(AsString()->value());

	default:
		UNREACHABLE();
	}
}
