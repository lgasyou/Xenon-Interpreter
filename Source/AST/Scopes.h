#pragma once

#include "Stable.h"
#include "AstValue.h"
#include "Utils/Zone.h"
#include <map>

class Variable;
class Declaration;
class DeclarationScope;

class Scope : public ZoneObject {
public:
	typedef std::map<std::string, AstValue> VariableMap;
	typedef std::string String;

	DeclarationScope *AsDeclarationScope();

	void AddInnerScope(Scope *inner) {
		inner->sibling_ = inner_scope_;
		inner_scope_ = inner;
		inner->outer_scope_ = this;
	}

	Scope *outerScope() const { return outer_scope_; }
	Scope *innerScope() const { return inner_scope_; }
	Scope *sibling() const { return sibling_; }

protected:
	Scope();

protected:
	// Scope tree.
	Scope *outer_scope_;
	Scope *inner_scope_;
	Scope *sibling_;

	VariableMap variables_;
};


class DeclarationScope : public Scope {
public:
	DeclarationScope();

	// Lookup a variable in this scope or outer scopes.
	// Returns the variable or NULL if not found.
	Variable *lookup(const String &name);

	// Lookup a variable in this scope. Returns the variable or NULL if not found.
	Variable *lookupLocal(const String &name);

	Variable *declarateVariable(Declaration *decl);

	// Declare a local variable in this scope. If the variable has been
	// declared before, the previously declared variable is returned.
	Variable *declarateLocal(Declaration *decl);

	// Find the first function, script, eval or (declaration) block scope. This is
	// the scope where var declarations will be hoisted to in the implementation.
	DeclarationScope* getDeclarationScope();
};
