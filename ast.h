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

class ProtectionType : public Node{
};

class Type : public Node{
};

class Literal : public Type{
};

class RunType : public Node{
};



class NormalRun : public RunType{
    public:
    const string kind = "Normal";
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

class PublicLiteral : public ProtectionType {
    public:
    const string kind = "public";
};

class PrivateLiteral : public ProtectionType {
    public:
    const string kind = "private";
};

class ProtectedLiteral : public ProtectionType {
    public:
    const string kind = "protected";
};

class Identifier : public Node{
    public:
    string value;
    Identifier(string v) : value(v) {}
};

class MainClassDef : public Node {
    public:
    const string kind = "main";
    vector<Node*> body;
    ProtectionType* protection;
    RunType* runtype;
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
