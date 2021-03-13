//
// Created by 欧阳世勇 on 2021/3/10.
//
#pragma once

#include <compiler/symtab.h>
#include "token.h"
#include "error.h"

class Parser {
public:
    void program();

    void segment();

    Tag type();

    // declaration and definition
    Var* defdata(bool ext,Tag t);

    void deflist(bool ext,Tag t);

    Var* vardef(bool ext,Tag t,bool ptr,string name);

    Var* init(bool ext,Tag t,bool ptr,string name);

    void def(bool b, Tag tag);

    void idtail(bool ext,Tag t,bool ptr,string name);

    // function
    Var* paradatatail(Tag t,string n);

    Var* paradata(Tag t);

    void para(vector<Var*> &paraList);

    void paralist(vector<Var*> &);

    void funtail(Func *func);

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

    void casestat(Var *cond);

    Var* caselabel();

    //expression
    Var* altexpr();

    Var* expr();

    Var* assexpr();

    Var* asstail(Var* lval);

    Var* orexpr();

    Var* ortail(Var* lval);

    Var* andexpr();

    Var* andtail(Var* lval);

    Var* cmpexpr();

    Var* cmptail(Var* lval);

    Tag cmps();

    Var* aloexpr();

    Var* alotail(Var* lval);

    Tag adds();

    Var* item();

    Var* itemtail(Var *lval);

    Tag muls();

    Var* factor();

    Tag lop();

    Var* val();

    Tag rop();

    Var* elem();

    Var* literal();

    Var* idexpr(string name);

    void realarg(vector<Var*> &args);

    void arglist(vector<Var*> &args);

    Var* arg();

    // LL(1) grammar pre-reading next character
    Token *look;
    Lexer &lexer;

    SymTab &symtab;

    void move();

    bool match(Tag need); // if match successful, move it.

    //programmer and recovery from error
    void recovery(bool cond, SynError lost, SynError wrong);

public:
    Parser(Lexer &lexer, SymTab &symTab);

    void analyse();

};


