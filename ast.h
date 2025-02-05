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

class Literal : public Node{
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
    vector<Node> body;
    ProtectionType protection;
    const Identifier name = Identifier("Main");
    MainClassDef(ProtectionType pt, vector<Node> b) : protection(pt), body(b) {}
};




#endif