//
// Created by 欧阳世勇 on 2021/3/10.
//
#include "parser.h"
#include "token.h"
#include "compiler/symtab.h"
#include <iostream>

using namespace std;
#define F(C) look->tag==C
#define _(T) ||look->tag==T
#define TYPE_FIRST F(KW_INT)_(KW_CHAR)_(KW_VOID)
#define EXPR_FIRST F(LPAREN)_(NUM)_(CH)_(STR)_(ID)_(NOT)_(SUB)_(LEA)_(MUL)_(INC)_(DEC)
#define LVAL_OPR F(ASSIGN)_(OR)_(AND)_(GT)_(GE)_(LT)_(LE)_(EQU)_(NEQU)_(ADD)_(SUB)_(MUL)_(DIV)_(MOD)_(INC)_(DEC)
#define RVAL_OPR F(OR)_(AND)_(GT)_(GE)_(LT)_(LE)_(EQU)_(NEQU)_(ADD)_(SUB)_(MUL)_(DIV)_(MOD)
#define STATEMENT_FIRST (EXPR_FIRST)_(SEMICON)_(KW_WHILE)_(KW_FOR)_(KW_DO)_(KW_IF)_(KW_SWITCH)_(KW_RETURN)_(KW_BREAK)_(KW_CONTINUE)

// <program> -> <segment> <program> | ^
void Parser::program() {
    if (F(END)) {
        return;
    }
    segment();
    return program();
}

//<segment> -> extern <type> <def> | <type> <def>
void Parser::segment() {
    bool ext = match(KW_EXTERN);
    Tag t = type();
    return def(ext, t);
}

// <type> -> INT | CH | VOID
// After handle with this function, it will have the type whatever variable or function
Tag Parser::type() {
    // default type is <int>
    Tag t = KW_INT;
    //FIRST(<type>) = INT | CHAR | VOID = TYPE_FIRST
    if (TYPE_FIRST) {
        t = look->tag;
        move();
    } else {
        // error
        // FOLLOW(<type>)= ID | MUL
        recovery(F(ID) _(MUL), TYPE_LOST, TYPE_WRONG);
    }
    return t;
}

// <def> -> ID <idtail> | MUL ID <init> <deflist>
void Parser::def(bool ext, Tag t) {
    string name = "";
    // FIRST(<def>) = ID | MUL
    // variable
    if (F(ID)) {
        name = ((Id *) look)->name;
        move();
        idtail(ext, t, false, name);
        return;
    }
    // pointer
//    else if (F(MUL)) {
//        move();
//        if (F(ID)) {
//            name = (((Id *) look)->name);
//            move();
//            // define a pointer here
//            init(ext, t, true, name);
//            deflist();
//            return ;
//        }
    if (match(MUL)) {
        if (F(ID)) {
            name = (((Id *) look)->name);
            move();
            // define a pointer here
            init(ext, t, true, name);
            deflist(ext, t);
            return;
        }
        // FIRST(<init>) = = | ^
        // FIRST(<deflist>) = , | ;
        recovery(F(ASSIGN) _(COMMA) _(SEMICON), ID_LOST, ID_WRONG);
    }
    //error
    // FIRST(<idtail>) = [ | = | , | ; | (
    recovery(F(LBRACK) _(ASSIGN) _(COMMA) _(SEMICON) _(LPAREN), ID_LOST, ID_WRONG);
}

// <idtail> -> <vardef> <deflist> | ( <para> ) <funtail>
void Parser::idtail(bool ext, Tag t, bool ptr, string name) {
    // function
    if (match(LPAREN)) {
        symtab.enter();

        // parameters
        vector<Var *> paralist;
        para(paralist);
        if (!match(RPAREN)) {
            // FIRST(<funtail>) = ; | {
            recovery(F(LBRACK) _(SEMICON), RPAREN_LOST, RPAREN_WRONG);
        }
        // define a function here
        Func *func = new Func(ext, t, name, paralist);
        funtail(func);

        symtab.leave();
        return;
    }
    // variable
    symtab.addVar(vardef(ext, t, ptr, name));
    deflist(ext, t);
}

// <init> -> = <expr> | ^
Var *Parser::init(bool ext, Tag t, bool ptr, string name) {
    Var *initVal = NULL;
    // FIRST(<init>) = = | ^
    if (match(ASSIGN)) {
        initVal = expr();
        // create a initialized  variable here
    }
    // create a un-initialized variable
    return new Var(symtab.getScopePath(), ext, t, ptr, name, initVal);
}


// <deflist> -> , <defdata> <deflist> | ;
void Parser::deflist(bool ext, Tag t) {
    if (match(SEMICON)) {
        // it is the last variable
        return;
    }
    if (!match(COMMA)) {

        if (F(ID) _(MUL)) {
            // FIRST(<defdata>) = ID | MUL
            recovery(true, COMMA_LOST, COMMA_WRONG);
        } else {
            // FOLLOW(<deflist>) = FOLLOW(<def>) U FOLLOW(<idtail>) U FOLLOW(<localdef>)
            // FOLLOW(<def>) = FOLLOW(<segment>) = extern | TYPE_FIRST | #
            // FOLLOW(<idtail>) = FOLLOW(<def>) = extern | TYPE_FIRST | #
            // FOLLOW(<localdef>) =  } | TYPE_FIRST | STATEMENT_FISRT | EXPR_FIRST | ;
            recovery(TYPE_FIRST || STATEMENT_FIRST || F(KW_EXTERN) _(RBRACE), SEMICON_LOST, SEMICON_WRONG);
            return;
        }
    }
    symtab.addVar(defdata(ext, t));
    return deflist(ext, t);
}

// <vardef> -> [NUM] | <init>
// After this function, it will define a variable or array
Var *Parser::vardef(bool ext, Tag t, bool ptr, string name) {
    // FIRST(<vardef>) = [ | = | ^
    if (!match(LBRACK)) {
        return init(ext, t, ptr, name);
    }
    int len = 0;
    if (F(NUM)) {
        len = ((Num *) look)->val;
        move();
    } else {
        recovery(F(RBRACK), NUM_LOST, NUM_WRONG);
    }
    if (!match(RBRACK)) {
        // <idtail> -> <vardef> <deflist> | ( <para> ) <funtail>
        // FIRST(<deflist>) = , | ;
        recovery(F(COMMA) _(SEMICON), RBRACK_LOST, RBRACK_WRONG);
    }
    // define a array here
    return new Var(symtab.getScopePath(), ext, t, name, len);
}


// <funtail> -> ; | <block>
void Parser::funtail(Func *func) {
    if (match(SEMICON)) {
        // a declaration of function
        symtab.decFunc(func);
    } else {
        symtab.defFunc(func);
        // a definition of function
        block();
        symtab.endDefFunc();
    }
}

// <defdata> -> id <vardef>|mul id <init>
Var *Parser::defdata(bool ext, Tag t) {
    string name = "";
    if (F(ID)) { // after get the ID, it it the variable name
        name = ((Id *) look)->name;
        move();
        return vardef(ext, t, false, name);
    }
    if (match(MUL)) {
        if (F(ID)) {
            name = ((Id *) look)->name;
            move();
        } else {
            // <deflist> -> , <defdata> <deflist> | ;
            // FIRST(<deflist>) = , | ;
            // FIRST(<init>) = = | ^
            recovery(F(SEMICON) _(COMMA) _(ASSIGN), ID_LOST, ID_WRONG);
        }
        return init(ext, t, true, name);
    } else {
        // <deflist> -> , <defdata> <deflist> | ;
        // FIRST(<deflist>) = , | ;
        // FIRST(<vardef>) = [ | = | ^
        recovery(F(SEMICON) _(COMMA) _(ASSIGN) _(LBRACK), ID_LOST, ID_WRONG);
        return vardef(ext, t, false, name);
    }
}

// <para> -> <type> <paradata> <paraList> | ^
void Parser::para(vector<Var *> &paraList) {
    // FOLLOW(<para>) = )
    if (F(RPAREN)) {
        return;
    }
    Tag t = type();
    Var *tmp = paradata(t);
    symtab.addVar(tmp);
    paraList.push_back(tmp);
    paralist(paraList);
}

// <paradata> -> MUL ID | ID <paradatatail>
Var *Parser::paradata(Tag t) {
    string name = "";
    if (match(MUL)) {
        if (F(ID)) {
            name = ((Id *) look)->name;
            move();
        } else {
            // <para> -> <type> <paradata> <paralist> | ^
            // FOLLOW(<para>) = )
            // FIRST(<paralist>) = , | ^
            recovery(F(COMMA) _(RPAREN), ID_LOST, ID_WRONG);
        }
        // define a pointer here
        return new Var(symtab.getScopePath(), false, t, true, name);
    }
    if (F(ID)) {
        name = ((Id *) look)->name;
        move();
    } else {
        // FIRST(<paradatatail>) = [ | ^
        // <para> -> <type> <paradata> <paralist> | ^
        // FOLLOW(<para>) = )
        // FIRST(<paralist>) = , | ^
        recovery(F(COMMA) _(RPAREN) _(LBRACK), ID_LOST, ID_WRONG);
        return new Var(symtab.getScopePath(), false, t, true, name);
    }
    return paradatatail(t, name);
}

// <paralist> -> , <type><paradata> <paralist> | ^
void Parser::paralist(vector<Var *> &args) {
    // FIRST(<paralist>) = , | ^
    if (match(COMMA)) {
        Tag t = type();
        Var *tmp = paradata(t);
        symtab.addVar(tmp);
        args.push_back(tmp);
        paralist(args);
    }
}

// <paradatatail> -> [NUM] | ^
Var *Parser::paradatatail(Tag t, string n) {
    // FIRST(<paradatatail>) = [ | ^
    if (match(LBRACK)) {
        int len = 1;
//        if (F(NUM)) {
//            len = ((Num *) look)->val;
//            move();
//            if (!match(RBRACK)) {
//                recovery(F(COMMA)_(RPAREN),RBRACK_LOST,RBRACK_WRONG);
//                return;
//            }
//            // define a array here
//        } else {
//            // allow the length is empty
//            if (!match(RBRACK)){
//                recovery(F(COMMA)_(RPAREN),RBRACK_LOST,RBRACK_WRONG);
//                return;
//            }
//        }
        if (F(NUM)) {
            len = ((Num *) look)->val;
            move();
        }
        // allow the length is empty
        if (!match(RBRACK)) {
            // <paradata> -> MUL ID | ID <paradatatail>
            // FOLLOW(<paradata>) = , | )
            recovery(F(COMMA) _(RPAREN), RBRACK_LOST, RBRACK_WRONG);
        }
        // define a array here
        return new Var(symtab.getScopePath(), false, t, n, len);
    }
    //define a variable here
    return new Var(symtab.getScopePath(), false, t, false, n);;
}


// <block> -> { <subprogram> }
void Parser::block() {
    // FIRST(<block>) = {
    if (!match(LBRACE)) {
        // FIRST(<subprogram>) = TYPE_FIRST | STATEMENT_FISRT | ^
        recovery(TYPE_FIRST || STATEMENT_FIRST || F(RBRACE), LBRACE_LOST, LBRACE_WRONG);
        return;
    }
    subprogram();
    if (!match(RBRACE)) {
        // when the <subprogram> is empty
        // <casestat> -> CASE <caselabel> :  <subprogram> <casestat> | DEFAULT : <subprogram>
        // FIRST(<casestat>) = CASE | DEFAULT

        // FOLLOW(<block>) = extern | TYPE_FIRST | #
        // <funtail> -> ; | <block>
        // FOLLOW(<funtail>) = extern | TYPE_FIRST | #
        // <whilestat> -> WHILE ( <altexpr> )  <block>
        // FOLLOW(<whilestat>)= TYPE_FIRST | STATEMENT_FISRT | }
        // <dowhilestat> -> DO <block> WHILE ( <altexpr> ) ;
        // <forstat> -> FOR ( <forinit> <altexpr> ; <altexpr> ) <block>
        // FOLLOW(<forstat>) = TYPE_FIRST | STATEMENT_FISRT | }
        // <ifstat> -> IF ( <expr> ) <block> <elsestat>
        // FIRST(<elsestat>) = ELSE | ^
        // FOLLOW(<ifstat>) = TYPE_FIRST | STATEMENT_FISRT | }
        // <elsestat> -> ELSE <block> | ^
        // FOLLOW(<elsestat>) = TYPE_FIRST | STATEMENT_FISRT | }
        recovery(TYPE_FIRST || STATEMENT_FIRST ||
                 F(KW_EXTERN) _(KW_ELSE) _(KW_CASE) _(KW_DEFAULT), RBRACE_LOST, RBRACE_LOST);
    }
}

// <subprogram> -> <localdef> <subprogram> | <statement> <subprogram> | ^
void Parser::subprogram() {
    // FIRST(<subprogram>) = TYPE_FIRST | STATEMENT_FISRT | ^
    if (TYPE_FIRST) {
        localdef();
        subprogram();
        return;
    }
    if (STATEMENT_FIRST) {
        statement();
        subprogram();
        return;
    }
}

// <localdef> -> <type> <defdata> <deflist>
void Parser::localdef() {
    Tag t = type();
    symtab.addVar(defdata(false, t));
    deflist(false, t);
}

// <expr> -> <assexpr>
Var *Parser::expr() {
    return assexpr();
}


//<assexpr> -> <orexpr> <asstail>
Var *Parser::assexpr() {
    Var *lval = orexpr();
    return asstail(lval);
}

//<asstail> -> = <orexpr> <asstail> | ^
Var *Parser::asstail(Var *lval) {
    if (!match(ASSIGN)) {
        return lval;
    }
    Var *rval = orexpr();
    // TODO:finish the assign oprate
    Var *res = rval;
    return asstail(res);
}

// <orexpr> -> <andexpr> <ortail>
Var *Parser::orexpr() {
    Var *lval = andexpr();
    return ortail(lval);
}

//<ortail> -> OR <andexpr> <ortail> | ^
Var *Parser::ortail(Var *lval) {
    if (!match(OR)) {
        return lval;
    }
    Var *rval = andexpr();
    // TODO :finish the OR operate
    Var *res;
    // Var *res = lval OR rval;
    return ortail(res);

}

//<andexpr> -> <cmpexpr> <andtail>
Var *Parser::andexpr() {
    Var *lval = cmpexpr();
    return andtail(lval);
}

//<andtail> -> AND <cmpexpr> <andtai> | ^
Var *Parser::andtail(Var *lval) {
    if (!match(AND)) {
        return lval;
    }
    Var *rval = cmpexpr();
    // TODO :finish the AND operate
    Var *res;
    // Var *res = lval AND rval;
    return andtail(res);
}

//<cmpexpr> -> <aloexpr> <cmptail>
Var *Parser::cmpexpr() {
    Var *lval = aloexpr();
    return cmptail(lval);
}

//<cmptail> -> <cmps> <aloexpr> <cmptail> | ^
Var *Parser::cmptail(Var *lval) {
    if (!(F(OR) _(AND) _(GT) _(GE) _(LT) _(LE) _(EQU) _(NEQU))) {
        return lval;
    }
    Tag op = cmps();
    Var *rval = aloexpr();
    // TODO :finish the OP operate
    Var *res;
    // Var *res = lval OP rval;
    return cmptail(res);
}

//<cmps> -> GT | GE | LT | LE | EQU | NEQU
Tag Parser::cmps() {
    Tag op = look->tag;
    // it must check the tag and come in
//    if (F(OR) _(AND) _(GT) _(GE) _(LT) _(LE) _(EQU) _(NEQU)) {
//
//    }
    move();
    return op;
}

//<aloexpr> -> <item> <alotail>
Var *Parser::aloexpr() {
    Var *lval = item();
    return alotail(lval);
}

//<alotail> -> <adds> <item> <alotail> | ^
Var *Parser::alotail(Var *lval) {
    if (!(F(ADD) _(SUB))) {
        return lval;
    }
    Tag op = adds();
    Var *rval = item();
    // TODO :finish the OP operate
    Var *res;
    // Var *res = lval OP rval;
    return alotail(res);
}

//<adds> -> ADD | SUB
Tag Parser::adds() {
//    if (F(ADD) _(SUB)) {
//        return
//    }
    Tag op = look->tag;
    move();
    return op;
}

//<item> -> <factor> <itemtail>
Var *Parser::item() {
    Var *lval = factor();
    return itemtail(lval);
}

//<itemtail> -> <muls> <factors> <itemtail> | ^
Var *Parser::itemtail(Var *lval) {
    if (!(F(MUL) _(DIV) _(MOD))) {
        return lval;
    }
    Tag op = muls();
    Var *rval = factor();
    // TODO :finish the OP operate
    Var *res;
    // Var *res = lval OP rval;
    return itemtail(res);
}

// <muls> -> MUL | DIV | MOD
Tag Parser::muls() {
//    if (F(MUL) _(DIV) _(MOD)) {
//
//    }
    Tag op = look->tag;
    move();
    return op;
}

//<factor> -> <lop> <factor> | <val>
Var *Parser::factor() {
    if (F(NOT) _(SUB) _(LEA) _(MUL) _(INC) _(DEC)) {
        Tag op = lop();
        Var *v = factor();
        // TODO :finish the OP operate
        Var *res;
        // Var *res = lval OP rval;
        return res;
    }
    return val();

}

//<lop> -> NOT | SUB | LEA | MUL | INCR | DECR
Tag Parser::lop() {
//    if (F(NOT) _(SUB) _(LEA) _(MUL) _(INC) _(DEC)) {
//    }
    Tag op = look->tag;
    move();
    return op;
}

// <val> -> <elem> <rop>
Var *Parser::val() {
    Var *val = elem();
    if (F(INC)_(DEC)){
        Tag op = rop();
        // generate intercode
    }
    // TODO :finish the OP operate

    // Var *res = lval OP rval;
    return val;
}

// <rop> -> INC | DEC
Tag Parser::rop() {
//    if (F(INC) _(DEC)) {
//
//    }
    Tag op = look->tag;
    move();
    return op;
}

// <elem> -> ID <idexpr> | ( <expr> ) | <literal>
Var *Parser::elem() {
    if (F(ID)) {
        string name = ((Id *) look)->name;
        move();
        return idexpr(name);
    }
    if (match(LPAREN)) {
        Var *v = expr();
        if (!match(RPAREN)) {
            // <val> -> <elem> <rop>
            // FOLLOW(<val>) = FOLLOW(<factor>) = MUL | DIV | MOD | ADD | SUB | GT | GE | LT | LE | EQU | NEQU | AND | = | OR | , | ; | extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST | ) | }
            // FIRST(<rop>) = INC | DEC
            recovery(LVAL_OPR, RPAREN_LOST, RPAREN_WRONG);
        }
        return v;
    }
    return literal();
}

// <idexpr> -> [ <expr> ] | ( <realarg> ) | ^
Var *Parser::idexpr(string name) {
    if (match(LBRACK)) {
        Var *v = expr();
        if (!match(RBRACK)) {
            // <elem> -> ID <idexpr> | ( <expr> ) | <literal>
            // FOLLOW(<elem>) = INC | DEC
            recovery(LVAL_OPR, LBRACE_LOST, LBRACE_WRONG);
        }
        return v;
    }
    if (match(LPAREN)) {
        vector<Var *> args;
        realarg(args);
        if (!match(RPAREN)) {
            recovery(RVAL_OPR, RPAREN_LOST, RPAREN_WRONG);
        }
        Func *func = symtab.getFunc(name, args);
        // TODO :finish the CALL FUNCTION
        Var *res;
        // Var *res = CallFunc(name,args)
        return res;
    }
    return symtab.getVar(name);
}

// <realarg> -> <arg> <arglist> | ^
void Parser::realarg(vector<Var *> &args) {
    // FIRST(<realarg>) = <arg> <arglist> | ^
    if (!(EXPR_FIRST)) {
        return;
    }
    args.push_back(arg());
    arglist(args);
}

// <arglist> -> , <arg> <arglist> | ^
void Parser::arglist(vector<Var *> &args) {
    // FIRST(<arglist>) = , | ^
    if (!match(COMMA)) {
        return;
    }
    args.push_back(arg());
    arglist(args);
}

// <arg> -> <expr>
Var *Parser::arg() {
    return expr();
}


// <literal> -> NUM | CH | STR
Var *Parser::literal() {
    if (!(F(NUM) _(CH) _(STR))) {
        recovery(RVAL_OPR, LITERAL_LOST, LITERAL_WRONG);
        return NULL;
    }
    Var *v = new Var(look);
    if (F(STR)) {
        symtab.addStr(v);
    } else {
        symtab.addVar(v);
    }
    move();
    return v;
}

// <altexpr> -> <expr> | ^
Var *Parser::altexpr() {
    // FIRST(<altexpr>) = EXPR_FIRST | ^
    if (EXPR_FIRST) {
        return expr();
    }
    return Var::getVoid();
}


// <statement> -> <altexpr> ; | <whilestat> | <forstat> | <dowhilestat> | <ifstat> | <switchstat> | BREAK | CONTINUE |RETURN ; | RETURN <altexpr> ;
void Parser::statement() {
    switch (look->tag) {
        case KW_WHILE:
            whilestat();
            break;
        case KW_FOR:
            forstat();
            break;;
        case KW_DO:
            dowhilestat();
            break;
        case KW_IF:
            ifstat();
            break;
        case KW_SWITCH:
            switchstat();
            break;
        case KW_BREAK:
            // TODO finish the BREAK
            move();
            if (!match(SEMICON)) {
                // FOLLOW(<statement>) = TYPE_FIRST | STATEMENT_FISRT | }
                recovery(TYPE_FIRST || STATEMENT_FIRST || F(LBRACE), SEMICON_LOST, SEMICON_WRONG);
            }
            break;
        case KW_CONTINUE:
            // TODO finish the CONTINUE
            move();
            if (!match(SEMICON)) {
                recovery(TYPE_FIRST || STATEMENT_FIRST || F(LBRACE), SEMICON_LOST, SEMICON_WRONG);
            }
            break;
        case KW_RETURN:
            // TODO finish the RETURN
            move();
            altexpr();
            if (!match(SEMICON)) {
                recovery(TYPE_FIRST || STATEMENT_FIRST || F(LBRACE), SEMICON_LOST, SEMICON_WRONG);
            }
            break;
        default:
            altexpr();
            if (!match(SEMICON)) {
                recovery(TYPE_FIRST || STATEMENT_FIRST || F(LBRACE), SEMICON_LOST, SEMICON_WRONG);
            }
    }
}

// <whilestat> -> WHILE ( <altexpr> )  <block>
void Parser::whilestat() {
    symtab.enter();
    if (match(KW_WHILE)) {

        if (match(LPAREN)) {
            Var *cond = altexpr();
            if (match(RPAREN)) {
                block();
            } else {
                // FIRST(<block>) = {
                recovery(F(LBRACE), RPAREN_LOST, RPAREN_WRONG);
            }
        } else {
            // FIRST(<altexpr>) = EXPR_FIRST | ^
            recovery(EXPR_FIRST || F(RPAREN), LPAREN_LOST, LPAREN_WRONG);
        }
    }
    symtab.leave();
}

// <dowhilestat> -> DO <block> WHILE ( <altexpr> ) ;
void Parser::dowhilestat() {
    // TODO generate the intercode
    symtab.enter();
    if (match(KW_DO)) {
        // FIRST(<block>) = {
        if (match(LBRACE)) {
            block();
        } else {
            // support a <block> just replaced by a <statement> in somewhere
            statement();
        }
        if (!match(KW_WHILE)) {
            recovery(F(LPAREN), WHILE_LOST, WHILE_WRONG);
        }
        if (!match(LPAREN)) {
            //FIRST(<aloexpr>) = EXPR_FIRST
            recovery(EXPR_FIRST || F(RPAREN), LPAREN_LOST, LPAREN_WRONG);
        }
        symtab.leave();
        altexpr();
        if (!match(RPAREN)) {
            recovery(F(SEMICON), RPAREN_LOST, RPAREN_WRONG);
        }
        if (!match(SEMICON)) {
            //FOLLOW(<dowhilestat>) = TYPE_FIRST | STATEMENT_FISRT | }
            recovery(TYPE_FIRST || STATEMENT_FIRST || F(RBRACE), SEMICON_LOST, SEMICON_WRONG);
        }
    }
}

// <forstat> -> FOR ( <forinit> <altexpr> ; <altexpr> ) <block>
void Parser::forstat() {
    symtab.enter();
    if (match(KW_FOR)) {
        if (!match(LPAREN)) {
            // FIRST(<forinit>) = TYPE_FIRST | EXPR_FIRST | ;
            recovery(TYPE_FIRST || STATEMENT_FIRST || F(SEMICON), LPAREN_LOST, LPAREN_WRONG);
        }
        forinit();
        Var *cond = altexpr();
        if (!match(SEMICON)) {
            //FIRST(<altexpr>) = EXPR_FIRST | ^
            recovery(EXPR_FIRST, SEMICON_LOST, SEMICON_WRONG);
        }
        altexpr();
        if (!match(RPAREN)) {
            // FIRST(<block>) = {
            recovery(F(LBRACE), RPAREN_LOST, RPAREN_WRONG);
        }
        // support a <block> just replaced by a <statement> in somewhere
        if (F(LBRACE)) {
            block();
        } else {
            statement();
        }
    }
    symtab.leave();
}

// <forinit> -> <localdef> | <altexpr> ;
void Parser::forinit() {
    // FIRST(<forinit>) = TYPE_FIRST | EXPR_FIRST | ;
    if (TYPE_FIRST) {
        return localdef();
    }
    altexpr();
    if (!match(SEMICON)) {
        // FOLLOW(<forinit>) =  EXPR_FIRST | ;
        recovery(EXPR_FIRST, SEMICON_LOST, SEMICON_WRONG);
    }
}

// <ifstat> -> IF ( <expr> ) <block> <elsestat>
void Parser::ifstat() {
    // TODO generate the intercode
    symtab.enter();
    if (match(KW_IF)) {
        if (!match(LPAREN)) {
            recovery(EXPR_FIRST, LPAREN_LOST, LPAREN_WRONG);
        }
        Var *cond = expr();
        if (!match(RPAREN)) {
            recovery(F(LBRACE), RPAREN_LOST, RPAREN_WRONG);
        }
        // support a <block> just replaced by a <statement> in somewhere
        if (F(LBRACE)) {
            block();
        } else {
            statement();
        }
        symtab.leave();
        if (F(KW_ELSE)) {
            elsestat();
        }
    }
}

// <elsestat> -> ELSE <block> | ^
void Parser::elsestat() {
    if (match(KW_ELSE)) {
        symtab.enter();
        if (F(LBRACE)) {
            block();
        } else {
            statement();
        }
        symtab.leave();
    }
}

// <switch> -> SWITCH ( <expr> ) { <casestat> }
void Parser::switchstat() {
    if (match(KW_SWITCH)) {
        symtab.enter();
        if (!match(LPAREN)) {
            recovery(EXPR_FIRST, LPAREN_LOST, LPAREN_WRONG);
        }
        Var *cond = expr();
        if (!match(RPAREN)) {
            recovery(F(LBRACE), RPAREN_LOST, RPAREN_WRONG);
        }
        if (!match(LBRACE)) {
            //FIRST(<casestat>) = CASE | DEFAULT
            recovery(F(KW_CASE) _(KW_DEFAULT), LBRACE_LOST, LBRACE_WRONG);
        }
        casestat(cond);
        if (!match(RBRACE)) {
            //FOLLOW(<switchstat>) = TYPE_FIRST | STATEMENT_FISRT | }
            recovery(TYPE_FIRST || STATEMENT_FIRST, RBRACE_LOST, RBRACE_WRONG);
        }
        symtab.leave();
    }
}

// <casestat> -> CASE <caselabel> :  <subprogram> <casestat> | DEFAULT : <subprogram>
void Parser::casestat(Var *cond) {
    if (match(KW_CASE)) {
        Var *lb = caselabel();
        if (!match(COLON)) {
            // FIRST(<subprogram>) = TYPE_FIRST | STATEMENT_FISRT | ^
            recovery(TYPE_FIRST || STATEMENT_FIRST, COLON_LOST, COLON_WRONG);
        }
        symtab.enter();
        subprogram();
        symtab.leave();
        // TODO generate the intercode
        casestat(cond);
    }
    if (match(KW_DEFAULT)) {
        if (!match(COLON)) {
            // FIRST(<subprogram>) = TYPE_FIRST | STATEMENT_FISRT | ^
            recovery(TYPE_FIRST || STATEMENT_FIRST, COLON_LOST, COLON_WRONG);
        }
        symtab.enter();
        subprogram();
        symtab.leave();
    }
}

// <caselabel> -> <literal>
Var *Parser::caselabel() {
    return literal();
}


void Parser::move() {
    look = lexer.tokenize();
    // following code is for testing
    switch (look->tag) {
        case ID:
            cout << ((Id *) look)->name.c_str() << endl;
            break;
        case NUM:
            cout << ((Num *) look)->val << endl;
            break;
        case CH:
            cout << ((Id *) look)->name.c_str() << endl;
            break;
        case STR:
            cout << ((Str *) look)->str.c_str() << endl;
            break;
        default:
            cout << tokenName[look->tag] << endl;
    }
}

bool Parser::match(Tag need) {
    if (look->tag == need) {
        move();
        return true;
    }
    return false;
}

void Parser::recovery(bool cond, SynError lost, SynError wrong) {
    if (cond) {
        SYNERROR(lost, look);
    } else {
        SYNERROR(wrong, look);
        move();
    }
}

Parser::Parser(Lexer &lexer, SymTab &symTab) : lexer(lexer), symtab(symTab) {

}

void Parser::analyse() {
    move();
    program();
}
