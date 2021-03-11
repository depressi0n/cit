//
// Created by 欧阳世勇 on 2021/3/10.
//
#pragma once
#include "token.h"
#include "error.h"

class Parser {
public:
    void program();

    void segment();

    Tag type();

    // declaration and definition
    void defdata();

    void deflist();

    void vardef();

    void init();

    void def(bool b, Tag tag);

    void idtail();

    // function
    void paradatatail();

    void paradata();

    void para();

    void paralist();

    void funtail();

    void block();

    void subprogram();

    void localdef();

    // statement
    void statement();

    void whilestat();

    void dowhilestat();

    void forstat();

    void forinit();

    void ifstat();

    void elsestat();

    void switchstat();

    void casestat();

    void caselabel();

    //expression
    void altexpr();

    void expr();

    void assexpr();

    void asstail();

    void orexpr();

    void ortail();

    void andexpr();

    void andtail();

    void cmpexpr();

    void cmptail();

    void cmps();

    void aloexpr();

    void alotail();

    void adds();

    void item();

    void itemtail();

    void muls();

    void factor();

    void lop();

    void val();

    void rop();

    void elem();

    void literal();

    void idexpr();

    void realarg();

    void arglist();

    void arg();

    // LL(1) grammar pre-reading next character
    Token *look;
    Lexer &lexer;
    //programmer and recovery from error
    void move();

    bool match(Tag need); // if match successful, move it.

    void recovery(bool cond, SynError lost, SynError wrong);

public:
    Parser(Lexer &lexer);

    void analyse();

};


