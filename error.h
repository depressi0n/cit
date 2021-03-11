//
// Created by 欧阳世勇 on 2021/3/10.
//

#pragma once

#include <lexer/scanner.h>
#include <lexer/token.h>

#define LEXERROR(code) Error::lexError(code)
#define SYNERROR(code, t) Error::synError(code,t)
#define SEMERROR(code, name) Error::semError(code,name)
#define SEMWARN(code, name) Error::semWarn(code,name)
class Scanner;
class Error {
    static Scanner *scanner;
public:
    Error(Scanner *sc);

    static int errorNum;  //errors number
    static int warnNum;  //warn number

    static void lexError(int code);

    static void synError(int code, Token *t);

    static void semError(int code, string name = "");

    static void semWarn(int code, string name = "");

    static int getErrorNum();

    static int getWarnNum();
};

enum LexError {
    STR_NO_R_QUTION, // the string have no right qution
    NUM_BIN_TYPE,
    NUM_HEX_TYPE,
    CHAR_NO_R_QUTION,// the character have no right qution
    CHAR_NO_DATA,
    OR_NO_PAIR,// just a | not ||
    COMMENT_NO_END, // multiply comment do not end normally
    TOKEN_NO_EXIST,
};

// 语法错误
enum SynError {
    TYPE_LOST,
    TYPE_WRONG,
    ID_LOST,
    ID_WRONG,
    NUM_LOST,
    NUM_WRONG,
    LITERAL_LOST,
    LITERAL_WRONG,
    COMMA_LOST,
    COMMA_WRONG,
    SEMICON_LOST,
    SEMICON_WRONG,
    ASSIGN_LOST,
    ASSIGN_WRONG,
    COLON_LOST,
    COLON_WRONG,
    WHILE_LOST,
    WHILE_WRONG,
    LPAREN_LOST,
    LPAREN_WRONG,
    RPAREN_LOST,
    RPAREN_WRONG,
    LBRACK_LOST,
    LBRACK_WRONG,
    RBRACK_LOST,
    RBRACK_WRONG,
    LBRACE_LOST,
    LBRACE_WRONG,
    RBRACE_LOST,
    RBRACE_WRONG,
};

// 语法错误
enum SemError {
    VAR_RE_DEF,                    //re-definition of variable
    FUN_RE_DEF,                    //re-definition of funtion
    VAR_UN_DEC,                    //no declaration of variable
    FUN_UN_DEC,                    //no declaration of function
    FUN_DEC_ERR,                //the declaration of function do not match the definition
    FUN_CALL_ERR,                //the parameters do not match
    DEC_INIT_DENY,            //can not initialize when declaration
    EXTERN_FUN_DEF,            //can use extern when declaration of function
    ARRAY_LEN_INVALID,    //invalid length of array
    VAR_INIT_ERR,                //type error when initialize the variable
    GLB_INIT_ERR,                //global variable do not use a constant to initialize
    VOID_VAR,                        //void error
    EXPR_NOT_LEFT_VAL,    //invalid left value of expression
    ASSIGN_TYPE_ERR,        //type error when assign
    EXPR_IS_BASE,                //expression can not be a fundamental type
    EXPR_NOT_BASE,            //expression can be a fundamental type
    ARR_TYPE_ERR,                //error in computation of array
    EXPR_IS_VOID,                //expression can not be a void type
    BREAK_ERR,                    //break is not in switch-case
    CONTINUE_ERR,                //continue is not in a cycle
    RETURN_ERR                    //the type of returned value do not match the return of function
};

// 语法警告
enum SemWarn {
    FUN_DEC_CONFLICT,        //conflict when call the function
    FUN_RET_CONFLICT        //conflict when return the function
};

#define FATAL "<fatal>:"
#define ERROR "<error>:"
#define WARN "<warn>:"


