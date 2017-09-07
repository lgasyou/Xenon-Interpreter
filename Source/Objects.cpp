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
	auto scope = block_->scope();
	for (int i = 0; i != formalArguments.size(); ++i) {
		auto fa = formalArguments[i];
		auto aa = actual_arguments_[i];
		scope->declarateVariable(aa);
		auto variableName = aa->variableProxy()->variable()->name();
		auto var = scope->lookup(variableName, /* createIfNotFound */true);
		*var = fa;
	}
	return block_;
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
