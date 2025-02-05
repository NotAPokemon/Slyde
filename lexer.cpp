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
    Identifier,
    Equals,
    OpenParentheses, ClosedParentheses,
    OpenBracket, ClosedBracket,
    DoubleQuotes,
    SingleQuotes,
    BinaryOperator,
    EOF_Token,
    SemiColon,
    ERROR,
};


struct Token{
    TokenType kind;
    string value;
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
            result.emplace_back(at, TokenType::OpenParentheses);
        } else if (at == ')') {
            result.emplace_back(at, TokenType::ClosedParentheses);
        } else if (at == '{') {
            result.emplace_back(at, TokenType::OpenBracket);
        } else if (at == '}') {
            result.emplace_back(at, TokenType::ClosedBracket);
        } else if (at == ';') {
            result.emplace_back(at, TokenType::SemiColon);
        } else if (at == '=') {
            result.emplace_back(at, TokenType::Equals);
        } else if (at == '\''){
            result.emplace_back(at, TokenType::SingleQuotes);
        } else if (at == '"'){
            result.emplace_back(at, TokenType::DoubleQuotes);
        } else if (at == '+' || at == '-') {
            result.emplace_back(at, TokenType::BinaryOperator);
        } else if (at == '*' || at == '/') {
            result.emplace_back(at, TokenType::BinaryOperator);
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
                    while (isint(at) && i < src.size()){
                        num += at;
                        i++;
                        at = src.at(i);
                    }
                    result.emplace_back(num, TokenType::Number);
                } else {
                    result.emplace_back(num, TokenType::Integer);
                }
            
            } else if (isalpha(at)){
                string str = "";
                while (isalpha(at) && i < src.size()){
                    str += at;
                    i++;
                    at = src.at(i);
                }
                result.emplace_back(str, TokenType::Identifier);
            } else if (at == ' ' || at == '\n' || at == '\t') {

            } else {
                result.emplace_back(at, TokenType::ERROR);
                break;
            }
        }
    }


    result.emplace_back("", TokenType::EOF_Token);
    return result;
}
