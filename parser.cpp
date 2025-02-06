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
            Token at = tokens.at(index);

            if (at.kind == TokenType::FileType){
                index++;
                at = tokens.at(index);
                if (at.kind == TokenType::Runtype) {
                    result.runtype = new NormalRun(); //for now it will allways be normal
                } else{
                    cout << "Error at token " + at.value << " At index: " << index << endl;
                    break;
                }
            } else if (index == 0) {
                result.runtype = new NormalRun();
            } else if (tokens.at(index).kind == TokenType::Protection && !parseBody){
                if (tokens.at(index + 1).value == "Main"){
                    parseBody = true;
                    result.protection = tokens.at(index).value;
                    index++;
                } else {
                    cout << "Error at token \"" + tokens.at(index + 1).value << "\" At index: " << index << endl;
                    cout << "The correct way to create the main class is:" << endl;
                    cout << "(public|private) Main{\n\t\"Your code here\"\n}" << endl;
                    break;
                }
                
            } else if (parseBody){
                lastNode = parse_expr();
                result.body.push_back(lastNode);
            } else if (tokens.at(index).kind == TokenType::SemiColon){
            } else {
                cout << "Error at token " + at.value << "type: " << at.kind << " At index: " << index << endl;
                cout << "The correct way to create the main class is:" << endl;
                cout << "(public|private) Main{\n\t\"Your code here\"\n}" << endl;
                break;
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
        Node* binop = left;

        while (tokens.at(index).value == "+" || tokens.at(index).value == "-"){
            string op = tokens.at(index).value;
            index++;
            Node* right = parse_multiplicative_expr();
            BinaryExpr* newbinop = new BinaryExpr();
            newbinop->left = left;
            newbinop->right = right;
            newbinop->op = op;

            binop = newbinop;
        }

        return binop;
    }

    Node* parse_multiplicative_expr(){
        Node* left = parse_primary_expr();
        Node* binop = left;

        while (tokens.at(index).value == "/" || tokens.at(index).value == "*" || tokens.at(index).value == "%" ){
            string op = tokens.at(index).value;
            index++;
            Node* right = parse_multiplicative_expr();
            BinaryExpr* newbinop = new BinaryExpr();
            newbinop->left = left;
            newbinop->right = right;
            newbinop->op = op;

            binop = newbinop;
        }

        return binop;
    }

    Node* parse_primary_expr(){
        TokenType tk = tokens.at(index).kind;
        string v = tokens.at(index).value;
        Null* n = new Null(); 

        if(tk == TokenType::IdentifierToken){
            index++;
            return new Identifier(tokens.at(index).value);
        } else if (tk == TokenType::Number){
            index++;
            return new NumberLiteral(stod(v));
        } else if (tk == TokenType::Integer){
            index++;
            return new IntegerLiteral(stoi(v));
        } else if(tk == TokenType::Boolean){
            index++;
            bool boolValue;
            switch (v){
                case "true":
                    boolValue = true;
                case "false":
                    boolValue = false;

            }
            return new BooleanLiteral(v);
        } else if (tk == TokenType::OpenParentheses){
            index++;
            Node* value = parse_expr();
            if (tokens.at(index).kind != TokenType::ClosedParentheses){
                cout << "Expected: ) but instead found (" << tokens.at(index).value << ")" << endl;
                return n;
            }
            index++;
            return value;
        } else if (tk == TokenType::typeDecleration) {
            VarDec* var = new VarDec();
            var->type = tokens.at(index).value;
            index++;
            Token indent = tokens.at(index);
            string Namevalue;
            if (indent.kind == TokenType::IdentifierToken){
                Namevalue = indent.value;
            } else {
                cout << "Expected: Identifier but instead found \"" << tokens.at(index).value << "\"" << endl;
                return n;
            }
            var->name = new Identifier(Namevalue);
            index++;
            if (tokens.at(index).kind != TokenType::Equals){
                Node* varValue = parse_expr();
                var->value = varValue;
            } else if (tokens.at(index).kind != TokenType::SemiColon){
            } else {
                if (tokens.at(index).kind != TokenType::ClosedParentheses){
                cout << "Expected: = but instead found \"" << tokens.at(index).value << "\"" << endl;
                return n;
            }
            }
            return var;
        }
            

        return n;
    }

};


string toString(MainClassDef main){
    string result = "{\n\tkind: \"Main\",\n\tprotection: \"" + main.protection + "\",";
    result += "\n\tbody: {\n";
    for (int i = 0; i < main.body.size(); i++){
        Node* at = main.body.at(i);
        if (isInstance<Identifier, Node>(at)){
            result += "\t\tIdentifier: \"" + static_cast<Identifier*>(at)->value + "\",";
        } else if (isInstance<BinaryExpr, Node>(at)){
            result += "\t\tBinaryExpr: \"" + static_cast<BinaryExpr*>(at)->op + "\",";
        } else if (isInstance<VarDec, Node>(at)){
            result += "\t\tVarDec: Type:\"" + static_cast<VarDec*>(at)->type +  + "\",";
        }

    }
    result += "}";
    return result;
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

    cout << toString(run) << endl;



    return 0;
}
