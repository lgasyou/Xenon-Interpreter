#pragma once

#include "Stable.h"
#include "AstValue.h"
#include "Utils/Zone.h"
#include "Objects.h"
#include <map>

class Variable;
class VariableDeclaration;
class FunctionDeclaration;
class Declaration;

class Scope : public ZoneObject {
public:
	typedef std::map<std::string, Object *> ObjectMap;
	typedef std::string String;

	Scope(Scope *outerScope = nullptr)
		: outer_scope_(outerScope) {}

	// Lookup a variable in this scope or outer scopes.
	// Returns the variable or variable which's just created if not found.
	Object *lookup(const String &name, bool createIfNotFound = true);

	// Lookup a variable in this scope. Returns the variable or NULL if not found.
	Object *lookupLocal(const String &name);

	// Declare a local variable in this scope. If the variable has been
	// declared before, this function'll do nothing.
	void declarateVariable(VariableDeclaration *decl);
	void declarateFunction(FunctionDeclaration *decl);
	void declarateLocal(Declaration *decl);

	void AddInnerScope(Scope *inner) {
		inner->sibling_ = inner_scope_;
		inner_scope_ = inner;
		inner->outer_scope_ = this;
	}

	Scope *outerScope() const { return outer_scope_; }
	Scope *innerScope() const { return inner_scope_; }
	Scope *sibling() const { return sibling_; }

protected:
	// Scope tree.
	Scope *outer_scope_;
	Scope *inner_scope_;
	Scope *sibling_;

	ObjectMap variables_;
};
