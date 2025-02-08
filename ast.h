#ifndef AST_H
#define AST_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include <unordered_map>

using namespace std;


class IndentManager {
public:
    string getIndent() {
        return string(level * 4, ' '); // 4 spaces per level
    }
    
    void increase() { level++; }
    void decrease() { level--; }
    int level = 0; // Static variable to track indentation level
};

template <typename Derived, typename Base>
bool isInstance(Base* node) {
    return dynamic_cast<Derived*>(node) != nullptr;
}

class Node{
    public:
    virtual ~Node() = default;
    virtual string toString(IndentManager m) const = 0;
};


class Blank : public Node{
    string toString(IndentManager m) const override{
        return "";
    }
};

class Null : public Node{
    string toString(IndentManager m) const override{
        return "Null";
    }
};

class Literal : public Node{
};


class RunType : public Node{
};

class NormalRun : public RunType{
    public:
    const string kind = "Normal";
    string toString(IndentManager m) const override{
        return kind;
    }
};

class MethodExpr : public Node{
    public:
    Node* params;
    string returnType;
};

class NumberLiteral : public Literal {
    public:
    double value;
    NumberLiteral(double v) : value(v) {}
    string toString(IndentManager m) const override{
        return to_string(value);
    }
};

class IntegerLiteral : public Literal {
    public:
    int value;
    IntegerLiteral(int v) : value(v) {}
    string toString(IndentManager m) const override{
        return to_string(value);
    }
};

class BooleanLiteral : public Literal{
    public:
    bool value;
    BooleanLiteral(bool v) : value(v) {}
    string toString(IndentManager m) const override{
        return value ? "true" : "false";
    }
};

class Identifier : public Node{
    public:
    string value;
    Identifier(string v) : value(v) {}
    string toString(IndentManager m) const override{
        return value;
    }
};

class VarDec : public Node{
    public:
    string type;
    Identifier* name;
    Node* value;
    string toString(IndentManager m) const override{
        string result = m.getIndent() + "Variable Declaration: {\n";
    
        m.increase();
        result += m.getIndent() + "Name: \"" + name->toString(m) + "\",\n";
        result += m.getIndent() + "Type: \"" + type + "\",\n";
        if (isInstance<Literal, Node>(value)){
            result += m.getIndent() + "Value: " + value->toString(m) + "\n";
        } else {
            result += m.getIndent() + "Value: \n";
            m.increase();
            result += value->toString(m) + "\n";
            m.decrease();
        }
        m.decrease();
        
        result += m.getIndent() + "}";
        
        return result;
    }
    
};

class VarAssignment : public Node{

};

class MethodDec : public MethodExpr{
    public:
    Node* params;
    string returnType;
    vector<Node*> body;
    Identifier name;
    string Protection;
    string toString(IndentManager m) const override{
        return "fix MethodDec to string";
    }
};

class Enviorment : public Node{
    public:
    vector<VarDec*> vars;
    vector<MethodDec*> methods;
    Enviorment* parent;
    vector<Enviorment*> children;
    string toString(IndentManager m) const override{
        return "";
    }
};


class MainClassDef : public Node {
    public:
    const string kind = "main";
    vector<Node*> body;
    string protection;
    RunType* runtype;
    Enviorment* env;
    MainClassDef(){
        env = new Enviorment();
    }

    string toString(IndentManager m) const override{
        m.level = 0;
        string result = m.getIndent() + "{\n";
    
        m.increase();
        
        result += m.getIndent() + "kind = {\"" + kind + "\"},\n";
        result += m.getIndent() + "RunType: {" + runtype->toString(m) + "},\n";
        result += m.getIndent() + "Protection: {" + protection + "},\n";
        
        result += m.getIndent() + "Body: {\n";
        m.increase();
        
        for (Node* n : body) {
            if (!isInstance<Blank, Node>(n)){
                result += n->toString(m) + ",\n";
            }
        }
        
        m.decrease();
        result += m.getIndent() + "}\n";
        
        m.decrease();
        result += m.getIndent() + "}";

        return result;
    }

};

class BinaryExpr : public Node {
    public:
    string op;
    Node* left;
    Node* right;
    string toString(IndentManager m) const override{
        string result = m.getIndent() + "BinaryExpr: {\n";
    
        m.increase();
        result += m.getIndent() + "Left: " + left->toString(m) + ",\n";
        result += m.getIndent() + "Operator: \"" + op + "\",\n";
        result += m.getIndent() + "Right: " + right->toString(m) + "\n";
        m.decrease();
        
        result += m.getIndent() + "}";
        
        return result;
    }
};





#endif
