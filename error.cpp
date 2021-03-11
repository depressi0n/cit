//
// Created by 欧阳世勇 on 2021/3/10.
//

#include "error.h"
#include "lexer/scanner.h"


int Error::errorNum = 0;
int Error::warnNum = 0;
Scanner *Error::scanner = NULL;
int Error::getErrorNum() {
    return errorNum;
}

int Error::getWarnNum() {
    return warnNum;
}

Error::Error(Scanner *sc) {
    scanner = sc;
}

void Error::lexError(int code) {
    static const char *lexErrorTable[] = {
            "String missing closing parenthesis",//"字符串丢失右括号",
            "Binary numbers have no physical data",//"二进制数没有实体数据",
            "Hexadecimal numbers have no physical data",//"十六进制数没有实体数据",
            "Character is missing right single bracket",//"字符丢失右单括号",
            "Null characters are not supported",//"不支持空字符",
            "Wrong OR operator",//"错误的或运算符",
            "Multi-line comments did not end normally",//"多行注释没有正常结束",
            "The lexical token does not exist",//"词法记号不存在",
    };
    errorNum++;
    printf("%s<%d row,%d column> lexical error ： %s.\n", scanner->getFile(), scanner->getLineNum(),
           scanner->getColNum(),
           lexErrorTable[code]);
}

void Error::synError(int code, Token *t) {
    static const char *synErrorTable[] = {
            "Type",
            "Identity",
            "LengthOfArray",
            "Constant",
            "Comma",
            "Semicolon",
            "Assign",
            "Colon",
            "while",
            "{",
            "}",
            "[",
            "]",
            "(",
            ")",
    };
    errorNum++;
    if (code % 2 == 0)//lost
        printf("%s<Line %d> syntax error : lost %s before %s .\n", scanner->getFile(), scanner->getLineNum(),
               synErrorTable[code / 2], t->String().c_str());
    else//wrong
        printf("%s<Line %d> syntax error : not match %s at %s .\n", scanner->getFile(), scanner->getLineNum(),
               synErrorTable[code / 2], t->String().c_str());
}

void Error::semError(int code, string name) {
    static const char *semErrorTable[]{
            "Variable redefinition",//"变量重定义",
            "Function redefinition",//"函数重定义",
            "Variable is not declared",//"变量未声明",
            "Function not declared",//"函数未声明",
            "The function declaration does not match the definition",//"函数声明与定义不匹配",
            "Function line parameter argument does not match",//"函数行参实参不匹配",
            "Initialization is not allowed during variable declaration",//"变量声明时不允许初始化",
            "Function definition cannot declare extern",//"函数定义不能声明extern",
            "The length of the array should be a positive integer",//"数组长度应该是正整数",
            "Variable initialization type error",//"变量初始化类型错误",
            "The initial value of the global variable is not a constant",//"全局变量初始化值不是常量",
            "Variables cannot be declared as void types",//"变量不能声明为void类型",
            "Invalid lvalue expression",//"无效的左值表达式",
            "The type of assignment expression is incompatible",//"赋值表达式类型不兼容",
            "Expression operand cannot be a basic type",//"表达式运算对象不能是基本类型",
            "Expression operand is not a basic type",//"表达式运算对象不是基本类型",
            "Array index operation type error",//"数组索引运算类型错误",
            "The return value of void function cannot participate in expression operation",//"void的函数返回值不能参与表达式运算",
            "Break statement cannot appear outside of loop or switch statement",//"break语句不能出现在循环或switch语句之外",
            "continue cannot appear outside the loop",//"continue不能出现在循环之外",
            "The return statement and the function return value type do not match"//"return语句和函数返回值类型不匹配"
    };
    errorNum++;
    printf("%s<Line %d> semantic error : %s %s.\n", scanner->getFile(), scanner->getLineNum(), name.c_str(),
           semErrorTable[code]);
}

void Error::semWarn(int code, string name) {
    static const char *semWarnTable[] = {
            "Function parameter list type conflict",//"函数参数列表类型冲突",
            "Function return value type does not exactly match"//"函数返回值类型不精确匹配"
    };
    warnNum++;
    printf("%s<Line %d> semantic warning : %s %s.\n", scanner->getFile(), scanner->getLineNum(),
           name.c_str(), semWarnTable[code]);
}
