//
// Created by 欧阳世勇 on 2021/3/10.
//
#include "parser.h"
#include "token.h"
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
    program();
}

//<segment> -> extern <type> <def> | <type> <def>
void Parser::segment() {
    bool ext = match(KW_EXTERN);
    Tag t = type();
    def(ext, t);
}

// <type> -> INT | CH | VOID
// After handle with this function, it will have the type whatever variable or function
Tag Parser::type() {
    // default type is <int>
    Tag t = KW_INT;
    if (TYPE_FIRST) {
        t = look->tag;
        move();
    } else {
        // error
        recovery(F(ID) _(MUL), TYPE_LOST, TYPE_WRONG);
    }
    return t;
}

// <def> -> ID <idtail> | MUL ID <init> <deflist>
void Parser::def(bool b, Tag tag) {
    string name = "";
    if (match(ID)) {
        name = ((Id *) look)->name;
        idtail();
        return;
    } else if (F(MUL)) {
        move();
        if (F(ID) == false) {
            recovery(F(SEMICON) _(COMMA) _(LPAREN) _(LBRACK), ID_LOST, ID_WRONG);
        }
        name = (((Id *) look)->name);
        move();
        // define a pointer here
        init();
        deflist();
    } else {
        //error
    }
}

// <idtail> -> <vardef> <deflist> | ( <para> ) <funtail>
void Parser::idtail() {
    if (match(LPAREN)) {
        para();
        if (!match(RPAREN)) {
            recovery(F(LBRACK) _(SEMICON), RPAREN_LOST, RPAREN_WRONG);
        }
        // define a function here
        funtail();
    } else {
        vardef();
        deflist();
    }
}

// <init> -> = <expr> | ^
void Parser::init() {
    if (match(ASSIGN)) {
        expr();
        // create a initialized  variable here
        return;
    }

    // create a un-initialized variable
    return;
}


// <deflist> -> , <defdata> <deflist> | ;
void Parser::deflist() {
    if (match(SEMICON)) {
        // the last variable
        return;
    }
    if (!match(COMMA)) {
        recovery(TYPE_FIRST || STATEMENT_FIRST || F(KW_EXTERN) _(RBRACE), SEMICON_LOST, SEMICON_WRONG);
        return;
    }
    defdata();
    deflist();
    return;
}

// <vardef> -> [NUM] | <init>
// After this function, it will define a variable or array
void Parser::vardef() {
    if (!match(LBRACK)) {
        // define a variable here
        init();
        return;
    }

    int len = 0;
    if (F(NUM) == false) {
        recovery(F(RBRACK), NUM_LOST, NUM_WRONG);
    }
    len = ((Num *) look)->val;
    move();
    if (!match(RBRACK)) {
        recovery(F(COMMA) _(SEMICON), RBRACK_LOST, RBRACK_WRONG);
    }
    // define a array here
    return;
}


// <funtail> -> ; | <block>
void Parser::funtail() {
    if (match(SEMICON)) {
        // a declaration of function
    } else {
        // a definition of function
        block();
    }
}

// <defdata> -> id <vardef>|mul id <init>
void Parser::defdata() {
    string name = "";
    if (F(ID)) { // after get the ID, it it the variable name
        name = ((Id *) look)->name;
        move();
        vardef();
        return;
    }
    if (!match(MUL)) {
        recovery(F(SEMICON) _(COMMA) _(ASSIGN) _(LBRACK), ID_LOST, ID_WRONG);
        return;
    }
    if (F(ID) == false) {
        recovery(F(SEMICON) _(COMMA) _(LBRACK), ID_LOST, ID_WRONG);
    }
    // after get the ID, it it the pointer name
    name = ((Id *) look)->name;
    move();
    return init();
}

// <para> -> <type> <paradata> <paralist> | ^
void Parser::para() {
    if (F(RPAREN)) {
        return;
    }
    type();
    paradata();
    paralist();
}

// <paradata> -> MUL ID | ID <paradatatail>
void Parser::paradata() {
    string name = "";
    if (match(MUL)) {
        if (!match(ID)) {
            recovery(F(COMMA) _(RPAREN), ID_LOST, ID_WRONG);
            return;
        }
        name = ((Id *) look)->name;
        move();
        // define a pointer here
        return;
    }
    if (!match(ID)) {
        recovery(F(COMMA) _(RPAREN) _(LBRACK), ID_LOST, ID_WRONG);
        return;
    }
    name = ((Id *) look)->name;
    move();
    paradatatail();
}

// <paralist> -> , <type><paradata> <paralist> | ^
void Parser::paralist() {
    if (match(COMMA)) {
        type();
        paradata();
        paralist();
    }
}

// <paradatatail> -> [NUM] | ^
void Parser::paradatatail() {
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
            recovery(F(COMMA) _(RPAREN), RBRACK_LOST, RBRACK_WRONG);
            return;
        }
        // define a array here
        return;
    }
    //define a variable here
    return;
}


// <block> -> { <subprogram> }
void Parser::block() {
    if (!match(LBRACE)) {
        recovery(TYPE_FIRST || STATEMENT_FIRST || F(RBRACE), LBRACE_LOST, LBRACE_WRONG);
        return;
    }
    subprogram();
    if (!match(RBRACE)) {
        recovery(TYPE_FIRST || STATEMENT_FIRST ||
                 F(KW_EXTERN) _(KW_ELSE) _(KW_CASE) _(KW_DEFAULT), RBRACE_LOST, RBRACE_LOST);
        return;
    }
    return;
}

// <subprogram> -> <localdef> <subprogram> | <statement> <subprogram> | ^
void Parser::subprogram() {
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
    type();
    defdata();
    deflist();
}

// <expr> -> <assexpr>
void Parser::expr() {
    assexpr();
}


//<assexpr> -> <orexpr> <asstail>
void Parser::assexpr() {
    orexpr();
    asstail();
}

//<asstail> -> = <orexpr> <asstail> | ^
void Parser::asstail() {
    if (!match(ASSIGN)) {
        return;
    }
    orexpr();
    asstail();
}

// <orexpr> -> <andexpr> <ortail>
void Parser::orexpr() {
    andexpr();
    ortail();
}

//<ortail> -> OR <andexpr> <ortail> | ^
void Parser::ortail() {
    if (!match(OR)) {
        return;
    }
    andexpr();
    ortail();
    return;
}

//<andexpr> -> <cmpexpr> <andtail>
void Parser::andexpr() {
    cmpexpr();
    andtail();
}

//<andtail> -> AND <cmpexpr> <andtai> | ^
void Parser::andtail() {
    if (!match(AND)) {
        return;
    }
    cmpexpr();
    andtail();
    return;
}

//<cmpexpr> -> <aloexpr> <cmptail>
void Parser::cmpexpr() {
    aloexpr();
    cmptail();
}

//<cmptail> -> <cmps> <aloexpr> <cmptail> | ^
void Parser::cmptail() {
    if (!(F(OR) _(AND) _(GT) _(GE) _(LT) _(LE) _(EQU) _(NEQU))) {
        return;
    }
    cmps();
    aloexpr();
    cmptail();
    return;
}

//<cmps> -> GT | GE | LT | LE | EQU | NEQU
void Parser::cmps() {
    if (F(OR) _(AND) _(GT) _(GE) _(LT) _(LE) _(EQU) _(NEQU)) {
    }
    move();
    return;
}

//<aloexpr> -> <item> <alotail>
void Parser::aloexpr() {
    item();
    alotail();
}

//<alotail> -> <adds> <item> <alotail> | ^
void Parser::alotail() {
    if (!(F(ADD) _(SUB))) {

    }
    adds();
    item();
    alotail();
    return;
}

//<adds> -> ADD | SUB
void Parser::adds() {
    if (F(ADD) _(SUB)) {

    }
    move();
    return;
}

//<item> -> <factor> <itemtail>
void Parser::item() {
    factor();
    itemtail();
}

//<itemtail> -> <muls> <factors> <itemtail> | ^
void Parser::itemtail() {
    if (!(F(MUL) _(DIV) _(MOD))) {
    }
    muls();
    factor();
    itemtail();
    return;

}

// <muls> -> MUL | DIV | MOD
void Parser::muls() {
    if (F(MUL) _(DIV) _(MOD)) {

    }
    move();
    return;
}

//<factor> -> <lop> <factor> | <val>
void Parser::factor() {
    if (F(NOT) _(SUB) _(LEA) _(MUL) _(INC) _(DEC)) {
        lop();
        factor();
    } else {
        val();
    }

}

//<lop> -> NOT | SUB | LEA | MUL | INCR | DECR
void Parser::lop() {
    if (F(NOT) _(SUB) _(LEA) _(MUL) _(INC) _(DEC)) {
    }
    move();
    return;
}

// <val> -> <elem> <rop>
void Parser::val() {
    elem();
    rop();
}

// <rop> -> INC | DEC
void Parser::rop() {
    if (F(INC) _(DEC)) {

    }
    move();
    return;
}

// <elem> -> ID <idexpr> | ( <expr> ) | <literal>
void Parser::elem() {
    if (match(ID)) {
        idexpr();
    } else if (match(LPAREN)) {
        expr();
        if (!match(RPAREN)) {
            recovery(LVAL_OPR, RPAREN_LOST, RPAREN_WRONG);
        }
        return;
    } else {
        literal();
    }
}

// <idexpr> -> [ <expr> ] | ( <realarg> ) | ^
void Parser::idexpr() {
    if (match(LBRACK)) {
        expr();
        if (!match(RBRACK)) {
            recovery(LVAL_OPR, LBRACE_LOST, LBRACE_WRONG);
        }
        return;
    }
    if (match(LPAREN)) {
        realarg();
        return;
    }
    return;
}

// <realarg> -> <arg> <arglist> | ^
void Parser::realarg() {
    if (!(EXPR_FIRST)) {
        return;
    }
    arg();
    arglist();
    return;

}

// <arglist> -> , <arg> <arglist> | ^
void Parser::arglist() {
    if (!match(COMMA)) {
        return;
    }
    arg();
    arglist();
    return;
}

// <arg> -> <expr>
void Parser::arg() {
    expr();
}


// <literal> -> NUM | CH | STR
void Parser::literal() {
    if (!(F(NUM) _(CH) _(STR))) {
        recovery(RVAL_OPR, LITERAL_LOST, LITERAL_WRONG);
        return;
    }
    move();
    return;
}

// <altexpr> -> <expr> | ^
void Parser::altexpr() {
    if (EXPR_FIRST) {
        expr();
    } else {

    }
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
            move();
            if (match(SEMICON)) {
                break;
            } else {
                //error
            }
        case KW_CONTINUE:
            move();
            if (match(SEMICON)) {
                break;
            } else {
                //error
            }
        case KW_RETURN:
            move();
            if (match(SEMICON)) {
                break;
            } else {
                //error
            }
        default:
            altexpr();
            if (match(SEMICON)) {

            } else {
                //error
            }
    }
}

// <whilestat> -> WHILE ( <altexpr> )  <block>
void Parser::whilestat() {
    if (match(KW_WHILE)) {
        if (match(LPAREN)) {
            altexpr();
            if (match(RPAREN)) {
                block();
            } else {
                //error
            }
        } else {
            //error
        }
    }
}

// <dowhilestat> -> DO <block> WHILE ( <altexpr> ) ;
void Parser::dowhilestat() {
    if (match(KW_DO)) {
        block();
        if (match(KW_WHILE)) {
            if (match(LPAREN)) {
                altexpr();
                if (match(RPAREN)) {
                    if (match(SEMICON)) {

                    } else {
                        //error
                    }
                } else {
                    //error
                }
            } else {
                // error
            }
        } else {
            //error
        }
    } else {
        //error
    }
}

// <forstat> -> FOR ( <forinit> <altexpr> ; <altexpr> ) <block>
void Parser::forstat() {
    if (match(KW_FOR)) {
        if (match(LPAREN)) {
            forinit();
            altexpr();
            if (match(SEMICON)) {
                altexpr();
                if (match(RPAREN)) {
                    block();
                } else {
                    //error
                }
            } else {
                //error
            }
        } else {
            //error
        }
    } else {
        //error
    }
}

// <forinit> -> <localdef> | <altexpr> ;
void Parser::forinit() {
    if (TYPE_FIRST) {
        localdef();
    } else {
        altexpr();
        if (match(SEMICON)) {

        } else {
            //error
        }
    }
}

// <ifstat> -> IF ( <expr> ) <block> <elsestat>
void Parser::ifstat() {
    if (match(KW_IF)) {
        if (match(LPAREN)) {
            expr();
            if (match(RPAREN)) {
                block();
                elsestat();
            } else {
                //error
            }
        } else {
            //error
        }
    } else {
        //error
    }
}

// <elsestat> -> ELSE <block> | ^
void Parser::elsestat() {
    if (match(KW_ELSE)) {
        block();
    } else {
        return;
    }
}

// <switch> -> SWITCH ( <expr> ) { <casestat> }
void Parser::switchstat() {
    if (match(KW_SWITCH)) {
        if (match(LPAREN)) {
            expr();
            if (match(RPAREN)) {
                if (match(LBRACE)) {
                    casestat();
                    if (match(RBRACE)) {

                    } else {
                        //error
                    }
                } else {
                    //error
                }
            } else {
                //error
            }
        } else {
            //error
        }
    } else {
        //error
    }
}

// <casestat> -> CASE <caselabel> :  <subprogram> <casestat> | DEFAULT : <subprogram>
void Parser::casestat() {
    if (match(KW_CASE)) {
        caselabel();
        if (match(COLON)) {
            subprogram();
            casestat();
        }
    } else if (match(KW_DEFAULT)) {
        subprogram();
    } else {
        //error
    }
}

// <caselabel> -> <literal>
void Parser::caselabel() {
    literal();
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

Parser::Parser(Lexer &lexer) : lexer(lexer) {

}

void Parser::analyse() {
    move();
    program();
}
