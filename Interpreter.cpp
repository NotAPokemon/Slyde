#include "parser.h"
#include "ast.h"
#include "lexer.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>



class Interpreter{
    public:
    MainClassDef execution;

    void execute(MainClassDef execution){

    }
};


int main(){
    ifstream file("./test.sly");

    Parser parser;

    string sourceCode;
    string data;
    while(getline(file, data)){
        sourceCode += data;
    }

    vector<Token> tokens = tokenize(sourceCode);

    MainClassDef run = parser.produceAST(tokens);

    MethodDec* println = new MethodDec();
    println->Protection = "public";
    println->name = new Identifier("println");
    println->returnType = "reserved";

    run.env->methods.push_back(println);

    VarDec* out = new VarDec();
    out->value = new StringLiteral("");

    run.env->vars.push_back(out);


    IndentManager manager = IndentManager();

    cout << run.toString(manager) << endl;

    



    return 1;
}