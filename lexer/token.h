//
// Created by 欧阳世勇 on 2021/3/10.
//

#pragma once

#include <cstdio>
#include <string>

using namespace std;
enum Tag {
    ERR,//error
    END,//end of file
    ID,//identity
    NUM, CH, STR,//constant
    KW_INT, KW_CHAR, KW_VOID,//data type
    KW_EXTERN,//extern
    NOT, LEA, // ! & - *
    ADD, SUB, MUL, DIV, MOD, // + - * /
    INC, DEC, // ++ --
    GT, GE, LT, LE, EQU, NEQU, // > >= < <= == !=
    AND, OR, // && ||
    LPAREN, RPAREN, // ()
    LBRACK, RBRACK,//[]
    LBRACE, RBRACE,//{}
    COMMA, COLON, SEMICON, //, : ;
    ASSIGN,// =
    KW_IF, KW_ELSE, //if-else
    KW_SWITCH, KW_CASE, KW_DEFAULT, //switch-case
    KW_WHILE, KW_DO, KW_FOR, // while do for
    KW_BREAK, KW_CONTINUE, KW_RETURN // break continue,return
};
extern const char *tokenName[];
class Token {
public:
    Tag tag;

    Token(Tag t);

    virtual string String();

    virtual ~Token() {};
};

// identity
class Id : public Token {
public:
    string name;

    Id(string n);

    virtual string String();
};

class Num : public Token {

    Num(int v);

    virtual string String();

public:
    int val;
};

class Char : public Token {

    virtual string String();

public:
    Char(char c);

    char ch;
};

class Str : public Token {

    virtual string String();

public:
    Str(string s);

    string str;
};