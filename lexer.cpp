#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include <unordered_map>


using namespace std;

enum TokenType {
    Number,
    Identifier,
    Equals,
    OpenParentheses, ClosedParentheses,


    BinaryOperator,

    Var,

    END,

    ERROR,

};

struct KeyWords{
    unordered_map<string, TokenType> words = {
        {"var", Var}
    };

    unordered_map<TokenType, string> TokenTypeNames = {
    {Number, "Number"},
    {Identifier, "Identifier"},
    {Equals, "Equals"},
    {OpenParentheses, "OpenParentheses"},
    {ClosedParentheses, "ClosedParentheses"},
    {BinaryOperator, "BinaryOperator"},
    {Var, "Var"},
    {END, "END"},
    {ERROR, "ERROR"}
};
};


struct Token{
    string value;
    TokenType type;
    Token(string value, TokenType type){
        this->value = value;
        this->type = type;
    }
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
    vector<Token> tokens;
    vector<char> src = split(sourceCode);
    KeyWords keywords;

    for (int i = 0; i < src.size(); i++){
        char at = src.at(i);
        if (at == '('){
            tokens.emplace_back(string(1, at), OpenParentheses);
        } else if (at == ')'){
            tokens.emplace_back(string(1, at), ClosedParentheses);
        } else if (at == '+' || at == '-'){
            tokens.emplace_back(string(1, at), BinaryOperator);
        } else if (at == '*' || at == '/') {
            tokens.emplace_back(string(1, at), BinaryOperator);
        } else if (at == '=') {
            tokens.emplace_back(string(1, at), Equals);
        } else {

            if (isint(at)){
                string num = "";
                while (isint(at) && i < src.size())
                {
                    num += at;
                    i++;
                    if (i < src.size()) at = src.at(i);
                }

                tokens.emplace_back(num, Number);
                
            } else if (isalpha(at)){
                string indentifier = "";
                while (isalpha(at) && i < src.size())
                {
                    indentifier += at; 
                    i++;
                    if (i < src.size()) at = src.at(i);
                }

                if (keywords.words.find(indentifier) != keywords.words.end()){
                    tokens.emplace_back(indentifier, keywords.words[indentifier]);
                } else {
                    tokens.emplace_back(indentifier, Identifier);
                }
                
            } else if (at == ' ' || at == '\t' || at == '\n'){
                
            } else {
                cout << "WTF did you type\n" << "we dont know how to handle this character " << i << endl;
                return {{"Error: unrecognized character" + at, ERROR}};
            }

        }


    }

    tokens.emplace_back("EOF", END);



    return tokens;
}





int main(){
    string filepath;
    cin >> filepath;
    if (filepath == ""){
        filepath = "./main.sly";
    }
    ifstream file(filepath);
    string code;
    KeyWords keywords;

    if (file.is_open()) {
        string data;
        while (getline(file, data)) {
            code += data;
        };
        file.close();
    } else {
        cout << "Error opening file!" << endl;
    }


    vector<Token> tokens = tokenize(code);
    for (int i = 0; i < tokens.size(); i++){
        cout << '{' << tokens.at(i).value << ',' << ((string) keywords.TokenTypeNames[tokens.at(i).type]) << "}\n";

    }


}