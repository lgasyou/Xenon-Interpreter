#include "Scope.h"
#include "Variable.h"
#include "AST/AST.h"

AstValue *Scope::lookupLocal(const String &name) {
	auto iter = variables_.find(name);
	auto end = variables_.end();
	return (iter != end) ? &((variables_.find(name))->second) : nullptr;
}

AstValue &Scope::lookup(const String &name) {
	AstValue *value = lookupLocal(name);
	if (value == nullptr) {
		if (outer_scope_) {
			return outer_scope_->lookup(name);
		}
		return variables_[name];
	}
	return *value;
}

void Scope::declarateVariable(VariableDeclaration *decl) {
	String name = decl->variableProxy()->variable()->name();
	variables_[name] = AstValue(decl->tokenType());
}
void Scope::declarateFunction(FunctionDeclaration *decl) {
	String name = decl->variableProxy()->variable()->name();
	UNREACHABLE();
	//functions_[name] = MCFunction(decl->tokenType());
}
void Scope::declarateLocal(Declaration *decl) {
	switch (decl->nodeType()) {
	case AstNode::VARIABLE_DECLARATION:
		declarateVariable(static_cast<VariableDeclaration*>(decl));
		break;
	case AstNode::FUNCTION_DECLARATION:
		declarateFunction(static_cast<FunctionDeclaration*>(decl));
		break;

	default:
		UNREACHABLE();
	}
}

