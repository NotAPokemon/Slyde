#ifndef AST_H
#define AST_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include <unordered_map>

using namespace std;

class Node{
    public:
    virtual ~Node() = default;
};

class Null : public Node{
};

class Literal : public Node{
};

class RunType : public Node{
};

class NormalRun : public RunType{
    public:
    const string kind = "Normal";
};

class MethodExpr : public Node{
    public:
    Node* params;
    string returnType;
};

class MethodDec : public MethodExpr{
    public:
    Node* params;
    string returnType;
    vector<Node*> body;
    Identifier name;
};

class NumberLiteral : public Literal {
    public:
    double value;
    NumberLiteral(double v) : value(v) {}
};

class IntegerLiteral : public Literal {
    public:
    int value;
    IntegerLiteral(int v) : value(v) {}
};

class Identifier : public Node{
    public:
    string value;
    Identifier(string v) : value(v) {}
};

class VarDec : public Node{
    public:
    string type;
    Identifier* name;
    Node* value;
};

class VarAssignment : public Node{

};

class MainClassDef : public Node {
    public:
    const string kind = "main";
    vector<Node*> body;
    string protection;
    RunType* runtype;
    vector<VarDec*> vars;
    vector<MethodDec*> methods;
    const Identifier name = Identifier("Main");
    MainClassDef(){}
};

class BinaryExpr : public Node {
    public:
    string op;
    Node* left;
    Node* right;
};

template <typename Derived, typename Base>
bool isInstance(Base* node) {
    return dynamic_cast<Derived*>(node) != nullptr;
}



#endif
