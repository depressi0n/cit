//
// Created by 欧阳世勇 on 2021/3/12.
//

#include "compiler.h"
#include "lexer/scanner.h"
#include "lexer/parser.h"
#include "error.h"
#include "symtab.h"
void Compiler::compile(char *filename) {
    Scanner* scanner=new Scanner(filename);
    Error *error=new Error(scanner);
    Lexer *lexer=new Lexer(*scanner);
    SymTab symtab;
    Parser *parser=new Parser(*lexer,symtab);
    parser->analyse();
}
