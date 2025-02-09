#include "parser.h"
#include "ast.h"
#include "lexer.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>


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


    IndentManager manager = IndentManager();

    cout << run.toString(manager) << endl;



    return 1;
}