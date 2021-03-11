//
// Created by 欧阳世勇 on 2021/3/10.
//
#pragma once

#include <unordered_map>
#include "token.h"
#include "error.h"
using namespace std;

class Scanner {
    char *fileName;
    FILE *file;

    static const int BUFLEN = 80;
    char line[BUFLEN];
    int lineLen;
    int cur;

    char lastCh;

    int lineNum;
    int colNum;

public:
    Scanner(char *name);

    virtual ~Scanner();

    int scan();

    FILE *getFile() const;

    int getLineNum() const;

    int getColNum() const;


private:
    void showChar(char ch);
};

class keyword {
    unordered_map<string, Tag> keywords;
public:
    keyword();

    Tag getTag(string name);
};

class Lexer {
    static keyword keywords;

    Scanner &scanner; //read character
    char ch;

    bool next(char need=0);

    Token *token; // current token
public:
    Lexer(Scanner &sc);

    ~Lexer();

    Token *tokenize(); // match and parse the symbol
};
