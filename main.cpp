//
// Created by 欧阳世勇 on 2021/3/10.
//
#include "lexer/scanner.h"
#include "lexer/parser.h"
#include "error.h"
#include <iostream>
#include <cstring>

int main(int argc, char *argv[]) {
    char *filename = argv[1];
    Scanner *scanner = new Scanner(filename);
    Lexer *lexer = new Lexer(*scanner);
    Parser *parser = new Parser(*lexer);
    Error *error = new Error(scanner);
    parser->analyse();
    return 0;
}