#ifndef PARSER_H
#define PARSER_H


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
    MainClassDef result;
    Enviorment* currentEnv = result.env;



    MainClassDef produceAST(vector<Token> tokensArray){

        
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
        Node* var = parse_additive_expr();
        return var;
    }

    Node* parse_additive_expr(){
        Node* left = parse_multiplicative_expr();
        Node* binop = left;

        while (tokens.at(index).value == "+" || tokens.at(index).value == "-"){
            string op = tokens.at(index).value;
            index++;
            Node* right = parse_multiplicative_expr();
            BinaryExpr* newbinop = new BinaryExpr();
            newbinop->left = binop;
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
            newbinop->left = binop;
            newbinop->right = right;
            newbinop->op = op;

            binop = newbinop;
        }

        return binop;
    }

    Node* parse_decleration(string protection = "private", bool skip = true, bool parsePar = false){
        string type;
        if (skip){
            type = tokens.at(index).value;
        } else {
            index++;
            //later do some checks to see if its a user defined type for now we will pass everything
            type = tokens.at(index).value;
        }
        
        index++;
        Token indent = tokens.at(index);
        string Namevalue;
        if (indent.kind == TokenType::IdentifierToken){
            Namevalue = indent.value;
        } else {
            cout << "Expected: Identifier but instead found \"" << tokens.at(index).value << "\"" << endl;
            throw new runtime_error("");
        }
        Identifier* name = new Identifier(Namevalue);
        index++;
        if (tokens.at(index).kind == TokenType::Equals){
            VarDec* var = new VarDec();
            var->name = name;
            var->type = type;
            var->protection = protection;
            index++;
            Node* varValue = parse_expr();
            var->value = varValue;
            return var;
        } else if (tokens.at(index).kind == TokenType::SemiColon || tokens.at(index).kind == TokenType::Comma){
            VarDec* var = new VarDec();
            var->name = name;
            var->type = type;
            var->protection = protection;
            index++;
            return var;
        } else if ((parsePar && tokens.at(index).kind == TokenType::ClosedParentheses)){
            VarDec* var = new VarDec();
            var->name = name;
            var->type = type;
            var->protection = protection;
            return var;
        } else if (tokens.at(index).kind == TokenType::OpenParentheses){
            MethodDec* var = new MethodDec();
            var->returnType = type;
            var->env = new Enviorment();
            var->name = name;
            var->Protection = protection;
            currentEnv->children.push_back(var->env);
            var->env->parent = currentEnv;
            currentEnv = var->env;
            index++;
            Token at = tokens.at(index);
            
            while (at.kind != TokenType::ClosedParentheses)
            {
                Node* pararm = parse_decleration(protection, skip, true);
                at = tokens.at(index);
                
            }
            index++;
            if (tokens.at(index).kind == TokenType::OpenBracket){
                index++;
                while (at.kind != TokenType::ClosedBracket)
                {
                    var->body.push_back(parse_expr());
                    at = tokens.at(index);
                    if (at.kind == TokenType::SemiColon){
                        index++;
                    }
                    at = tokens.at(index);
                }
                index++;
                currentEnv = currentEnv->parent;
            } else {
                throw new runtime_error("Expected: { but instead found \"" + string(at.value) + "\"");
            }
            return var;


        } else {
            if (tokens.at(index).kind != TokenType::ClosedParentheses){
                cout << "Expected: ['(' | '=' | ';'] but instead found \"" << tokens.at(index).value << "\"" << endl;
                throw new runtime_error("");
            }
        }
    }

    void handleEnviormentSetup(Node* var){
        if (isInstance<VarDec, Node>(var)){
            currentEnv->vars.push_back(static_cast<VarDec*>(var));
        } else if (isInstance<MethodDec, Node>(var)){
            MethodDec* m = static_cast<MethodDec*>(var);
            currentEnv->methods.push_back(m);
        }
    }

    Node* parse_primary_expr(){
        TokenType tk = tokens.at(index).kind;
        string v = tokens.at(index).value;
        Blank* n = new Blank(); 

        if(tk == TokenType::IdentifierToken){
            index++;
            string n = v;
            if (tokens.at(index).kind == TokenType::OpenParentheses){
                vector<Node*> params;
                while (tokens.at(index).kind != TokenType::ClosedParentheses){
                    params.push_back(parse_expr());
                }
                return new MethodCall(params, new Identifier(n));
            } else {
                return new Identifier(n);

            }
        } else if (tk == TokenType::Number){
            index++;
            return new NumberLiteral(stod(v));
        } else if (tk == TokenType::Integer){
            index++;
            return new IntegerLiteral(stoi(v));
        } else if(tk == TokenType::Boolean){
            index++;
            bool boolValue = false;
            if(v == "true"){
                boolValue = true;
            }
            return new BooleanLiteral(boolValue);
        } else if (tk == TokenType::String){
            index++;
            return new StringLiteral(v);
        } else if (tk == TokenType::OpenParentheses){
            index++;
            Node* value = parse_additive_expr();
            if (tokens.at(index).kind != TokenType::ClosedParentheses){
                cout << "Expected: ) but instead found (" << tokens.at(index).value << ")" << endl;
                return n;
            }
            index++;
            return value;
        } else if (tk == TokenType::typeDecleration) {
            Node* var = parse_decleration();
            handleEnviormentSetup(var);
            return var;
        } else if (tk == TokenType::Protection) {
            Node* var = parse_decleration(v, false);
            handleEnviormentSetup(var);
            return var;
        } else if(tk == TokenType::ReturnToken){
            index++;
            Node* val = parse_expr();
            return new Return(val);
        }

        return n;
    }

};



#endif

