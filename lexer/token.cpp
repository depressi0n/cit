//
// Created by 欧阳世勇 on 2021/3/10.
//

#include <sstream>
#include "token.h"

const char *tokenName[] = {
        "error",
        "end",
        "identity",
        "numeric", "ch", "string", // constant
        "int", "char", "void",//data type
        "extern",//extern
        "!", "&",// !  &
        "+", "-", "*", "/", "%",//numeric operator
        "++", "--",
        ">", ">=", "<", "<=", "==", "!=",//compare operator
        "&&", "||",//logic operator
        "(", ")",//()
        "[", "]",//[]
        "{", "}",//{}
        ",", ":", ";",//comma,semicolon,colon
        "=",//assign
        "if", "else",//if-else
        "switch", "case", "default",//swicth-case-deault
        "while", "do", "for",//cycle
        "break", "continue", "return"//break,continue,return
};

Token::Token(Tag t) : tag(t) {
}

string Token::String() {
    return tokenName[tag];
}

Id::Id(string n) : Token(ID), name(n) {

}

string Id::String() {
    return Token::String() + name;
}

Num::Num(int v) : Token(NUM), val(v) {

}

string Num::String() {
    stringstream ss;
    ss << val;
    return string("[") + Token::String() + ss.str();
}

Char::Char(char c) : Token(CH), ch(c) {
}

string Char::String() {
    stringstream ss;
    ss << ch;
    return string("[") + Token::String() + ss.str();
}

Str::Str(string s) : Token(STR), str(s) {

}

string Str::String() {
    stringstream ss;
    ss << str;
    return string("[") + Token::String() + ss.str();
}
