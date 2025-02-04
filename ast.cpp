#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include <unordered_map>


using namespace std;

enum NodeType {
    Program,
    IntLiteral,
    StrLiteral,
    Identifier,
    BinaryExpr,
    CallExpr,
    UnaryExpr,
    FunctionCreation,
};


unordered_map<NodeType, string> NodeTypeToString = {
    {Program, "Program"},
    {IntLiteral, "IntLiteral"},
    {Identifier, "Identifier"},
    {BinaryExpr, "BinaryExpr"},
    {CallExpr, "CallExpr"},
    {UnaryExpr, "UnaryExpr"},
    {FunctionCreation, "FunctionCreation"},
    {StrLiteral, "StrLiteral"}
};


unordered_map<string, NodeType> StringToNodeType = {
    {"Program", Program},
    {"IntLiteral", IntLiteral},
    {"Identifier", Identifier},
    {"BinaryExpr", BinaryExpr},
    {"CallExpr", CallExpr},
    {"UnaryExpr", UnaryExpr},
    {"FunctionCreation", FunctionCreation},
    {"StrLiteral", StrLiteral}
};


class Stmt{
    public:
    static NodeType kind;
};

class Program : public Stmt{
    public:
    vector<Stmt> body;
};


class Expr : public Stmt{};

class Literal : public Expr{};

class BinaryExpr : public Expr {
    public:
    Expr left;
    Expr right;
    string oper;
};

class Identifier : public Expr {
    public:
    string symbol;
    // do this later to make OOP
    //Literal type;

};

class IntLiteral : public Literal {
    public:
    double value;
};

class StrLiteral : public Literal {
    public:
    string value;
};


NodeType IntLiteral::kind = NodeType::IntLiteral;
NodeType Identifier::kind = NodeType::Identifier;
NodeType BinaryExpr::kind = NodeType::BinaryExpr;
NodeType Program::kind = NodeType::Program;
NodeType StrLiteral::kind = NodeType::StrLiteral;





