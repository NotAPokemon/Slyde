#include "ast.h"
#include "lexer.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>


class Parser{

    public:
    int index;
    vector<Token> tokens;




    MainClassDef produceAST(vector<Token> tokensArray){

        MainClassDef result;
        this->tokens = tokensArray;
        Node* lastNode;
        bool parseBody = false;


        for (int i =0; i < tokens.size(); i++){
            index = i;
            Token at = tokens.at(i);

            if (at.kind == TokenType::FileType){
                i++;
                at = tokens.at(i);
                if (at.kind == TokenType::Runtype) {
                    result.runtype = new NormalRun(); //for now it will allways be normal
                } else{
                    cout << "Error at token " + at.value << " At index: " << i;
                    break;
                }
            } else if (i == 0) {
                result.runtype = new NormalRun();
            } else if (tokens.at(index).kind == TokenType::Protection && tokens.at(index + 1).value == "Main"){
                parseBody = true;
            } else if (parseBody){
                lastNode = parse_expr();
                result.body.push_back(lastNode);
            }
            i = index;
        }

        return result;
    }


    Node* parse_expr(){

        return parse_additive_expr();

    }

    Node* parse_additive_expr(){
        Node* left = parse_multiplicative_expr();
        BinaryExpr* binop;

        while (tokens.at(index).value == "+" || tokens.at(index).value == "-"){
            string op = tokens.at(index).value;
            index++;
            Node* right = parse_multiplicative_expr();
            binop = new BinaryExpr();
            binop->left = left;
            binop->right = right;
            binop->op = op;
        }

        return binop;
    }

    Node* parse_multiplicative_expr(){
        Node* left = parse_primary_expr();
        BinaryExpr* binop;

        while (tokens.at(index).value == "/" || tokens.at(index).value == "*" || tokens.at(index).value == "%" ){
            string op = tokens.at(index).value;
            index++;
            Node* right = parse_multiplicative_expr();
            binop = new BinaryExpr();
            binop->left = left;
            binop->right = right;
            binop->op = op;
        }

        return binop;
    }

    Node* parse_primary_expr(){
        TokenType tk = tokens.at(index).kind;
        string v = tokens.at(index).value;

        switch (tk)
        {
        case TokenType::IdentifierToken:
            index++;
            return new Identifier(tokens.at(index).value);
        case TokenType::Number:
            index++;
            return new NumberLiteral(stod(v));
        case TokenType::Integer:
            index++;
            return new IntegerLiteral(stoi(v));
        case TokenType::OpenParentheses:
            index++;
            Node* value = parse_expr();
            if (tokens.at(index).kind != TokenType::ClosedParentheses){
                cout << "Expected: ) but instead found (" << tokens.at(index).value << ")" << endl;
                break;
            }
            index++;
            return value;
        }


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

    cout << run.body.at(0) << endl;



    return 0;
}