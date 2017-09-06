#include "Objects.h"
#include "AST/AstValue.h"

AstValue MCFunction::operator()(const std::vector<AstValue> &formalArguments) {
	if (formalArguments.size() != actual_arguments_.size()) {
		UNREACHABLE();
	}
	for (int i = 0; i != formalArguments.size(); ++i) {
		
	}
}
