#include "Scopes.h"

DeclarationScope *Scope::AsDeclarationScope() {
	return static_cast<DeclarationScope *>(this);
}
