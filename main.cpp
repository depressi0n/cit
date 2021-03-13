//
// Created by 欧阳世勇 on 2021/3/10.
//
#include "lexer/scanner.h"
#include "lexer/parser.h"
#include "error.h"
#include <iostream>
#include <cstring>
#include <compiler/compiler.h>

int main(int argc, char *argv[]) {
    char *filename = argv[1];
    // parse the arguments
    Compiler complier;
    complier.compile(filename);
    return 0;
}