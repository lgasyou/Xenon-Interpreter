#pragma once

// Basic AstNode
class AstNode {

};

class Declaration : public AstNode {

};

class FunctionDeclaration : public Declaration {

};

class VariableDeclaration : public Declaration {

};

class Expression : public AstNode {

};

class Statement : public AstNode {

};