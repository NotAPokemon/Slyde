#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include <unordered_map>


using namespace std;

enum TokenType {
    Number,
    Integer,
    IdentifierToken,
    Equals,
    OpenParentheses, ClosedParentheses,
    OpenBracket, ClosedBracket,
    DoubleQuotes,
    SingleQuotes,
    BinaryOperator,
    EOF_Token,
    SemiColon,
    Runtype,
    FileType,
    Class,
    Protection,
    typeDecleration,
    null,
    ERROR,
};


unordered_map<string, TokenType> keyWords = {
    {"Null", TokenType::null},
    {"null", TokenType::null},
    {"RunType", TokenType::FileType},
    {"class", TokenType::Class},
    {"private", TokenType::Protection},
    {"public", TokenType::Protection},
    {"protected", TokenType::Protection},
    {"Normal", TokenType::Runtype},
    {"int", TokenType::typeDecleration},
    {"String", TokenType::typeDecleration},
    {"Number", TokenType::typeDecleration}
};

bool isKeyWord(string word){
    return (keyWords.find(word) != keyWords.end());
}

   


struct Token{
    TokenType kind;
    string value;

    Token(string b, TokenType k) : value(b), kind(k) {}
};


bool isalpha(char i){
    return tolower(i) != toupper(i);
}

bool isint(char i){
    int unicode = static_cast<int>(i);
    return unicode >= static_cast<int>('0') && unicode <= static_cast<int>('9');
}


vector<char> split(const string& src){
    vector<char> characters;
    for (char c : src) {
        characters.push_back(c);
    }
    return characters;
}

vector<Token> tokenize(string sourceCode){
    vector<char> src = split(sourceCode);
    vector<Token> result;
    
    for (int i = 0; i < src.size(); i++){
        char at = src.at(i);
        if (at == '('){
            result.emplace_back(string(1, at), TokenType::OpenParentheses);
        } else if (at == ')') {
            result.emplace_back(string(1, at), TokenType::ClosedParentheses);
        } else if (at == '{') {
            result.emplace_back(string(1, at), TokenType::OpenBracket);
        } else if (at == '}') {
            result.emplace_back(string(1, at), TokenType::ClosedBracket);
        } else if (at == ';') {
            result.emplace_back(string(1, at), TokenType::SemiColon);
        } else if (at == '=') {
            result.emplace_back(string(1, at), TokenType::Equals);
        } else if (at == '\''){
            result.emplace_back(string(1, at), TokenType::SingleQuotes);
        } else if (at == '"'){
            result.emplace_back(string(1, at), TokenType::DoubleQuotes);
        } else if (at == '+' || at == '-') {
            result.emplace_back(string(1, at), TokenType::BinaryOperator);
        } else if (at == '*' || at == '/' || at == '%') {
            result.emplace_back(string(1, at), TokenType::BinaryOperator);
        } else {
            if (isint(at)){
                string num = "";
                while (isint(at) && i < src.size()){
                    num += at;
                    i++;
                    at = src.at(i);
                }
                if (at == '.'){
                    num += at;
                    i++;
                    at = src.at(i);
                    while (isint(at) && i < src.size()){
                        num += at;
                        i++;
                        at = src.at(i);
                    }
                    result.emplace_back(num, TokenType::Number);
                } else {
                    result.emplace_back(num, TokenType::Integer);
                }
                i--;
            
            } else if (isalpha(at)){
                string str = "";
                while (isalpha(at) && i < src.size()){
                    str += at;
                    i++;
                    at = src.at(i);
                }
                if (isKeyWord(str)){
                    result.emplace_back(str, keyWords.at(str));
                } else {
                    result.emplace_back(str, TokenType::IdentifierToken);
                }
                i--;
            } else if (at == ' ' || at == '\n' || at == '\t') {

            } else {
                result.emplace_back(string(1, at), TokenType::ERROR);
                break;
            }
        }
    }


    result.emplace_back("", TokenType::EOF_Token);
    return result;
}



#endif