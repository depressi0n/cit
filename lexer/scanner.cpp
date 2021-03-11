//
// Created by 欧阳世勇 on 2021/3/10.
//
# pragma  once

#include "scanner.h"
#include "error.h"
#include "token.h"

Scanner::Scanner(char *name) : fileName(name) {
    file = fopen(name, "r");
    if (!file) {
        printf(FATAL"can't open the file named %s, please check the path of file.\n", name);
        Error::errorNum++;
    }
    fileName = name;

    // initialize status
    lineLen = 0;
    cur = -1;
    lastCh = 0;
    lineNum = 0;
    colNum = 0;
}

Scanner::~Scanner() {
    if (file) {
        delete fileName;
    }
    if (file) {
        fclose(file);
    }
}

FILE *Scanner::getFile() const {
    return file;
}

int Scanner::getLineNum() const {
    return lineNum;
}

int Scanner::getColNum() const {
    return colNum;
}

int Scanner::scan() {
    if (!file) {
        return -1;
    }
    if (cur == lineLen - 1) {
        lineLen = fread(line, 1, BUFLEN, file);
        if (lineLen == 0) {
            lineLen = 1;
            line[0] = -1;
        }
        //reset the cursor
        cur = -1;
    }
    cur++;
    char ch = line[cur];
    // update the status
    if (lastCh == '\n') {
        lineNum++;
        colNum = 0;
    }
    if (ch == -1) {
        fclose(file);
        file = NULL;
    } else if (ch != '\n') {
        colNum++;
    }
    lastCh = ch;
    return ch;
}

void Scanner::showChar(char ch) {
    if (ch == -1) printf("EOF");
    else if (ch == '\n') printf("\\n");
    else if (ch == '\t') printf("\\t");
    else if (ch == ' ') printf("<blank>");
    else printf("%c", ch);
    printf("\t\t<%d>\n", ch);
}


Lexer::~Lexer() {
    if (!token) {
        delete token;
    }
}

keyword::keyword() {
    keywords["int"] = KW_INT;
    keywords["char"] = KW_CHAR;
    keywords["void"] = KW_VOID;
    keywords["extern"] = KW_EXTERN;
    keywords["if"] = KW_IF;
    keywords["else"] = KW_ELSE;
    keywords["switch"] = KW_SWITCH;
    keywords["case"] = KW_CASE;
    keywords["default"] = KW_DEFAULT;
    keywords["while"] = KW_WHILE;
    keywords["do"] = KW_DO;
    keywords["for"] = KW_FOR;
    keywords["break"] = KW_BREAK;
    keywords["continue"] = KW_CONTINUE;
    keywords["return"] = KW_RETURN;
}

Tag keyword::getTag(string name) {
    return keywords.find(name) != keywords.end() ? keywords[name] : ID;
}

keyword Lexer::keywords;

Lexer::Lexer(Scanner &scanner) : scanner(scanner) {
    token = NULL;
    ch = ' ';
}

bool Lexer::next(char need) {
    ch = scanner.scan();
    if (need) {
        if (ch != need) {
            return false;
        }
        ch = scanner.scan();
        return true;
    }
    return true;
}

Token *Lexer::tokenize() {
    for (; ch != -1;) {
        // skip the blank character
        Token *t = NULL;
        // skip the blank character
        while (ch == ' ' || ch == '\n' || ch == '\t') {
            next();
        }
        //identity
        if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_') {
            string name = "";
            do {
                name.push_back(ch);
                next();
            } while (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_' || ch >= '0' && ch <= '9');
            Tag tag = keywords.getTag(name);
            if (tag == ID) {
                // it is a identity
                t = new Id(name);
            } else {
                // it is a keyword
                t = new Token(tag);
            }
        }
            // character
        else if (ch == '\'') {
            char c;
            next();
            if (ch == '\\') {
                next();
                if (ch == 'n') {
                    c = '\n';
                } else if (ch == 't') {
                    c = '\t';
                } else if (ch == '\\') {
                    c = '\\';
                } else if (ch == '0') {
                    c = '\0';
                } else if (ch == '\'') {
                    c = '\'';
                } else if (ch == -1 || ch == '\n') {
                    // error
                    t = new Token(ERR);
                } else {
                    c = ch;
                }
            } else if (ch == -1 || ch == '\n') {
                //error
            } else if (ch == '\'') {
                //error
            } else {
                c = ch;
            }
            if (!t) {
                if (next('\'')) {
                    t = new Char(c);
                } else {
                    //error
                }
            }
        }
            //string
        else if (ch == '"') {
            string str = "";
            while (!next('"')) {
                if (ch == '\\') {
                    next();
                    if (ch == 'n') {
                        str.push_back('\n');
                    } else if (ch == 't') {
                        str.push_back('\t');
                    } else if (ch == '\\') {
                        str.push_back('\\');
                    } else if (ch == '0') {
                        str.push_back('\0');
                    } else if (ch == '\n') {
                        // nothing
                    } else if (ch == -1) {
                        t = new Token(ERR);
                        // error
                    } else {
                        str.push_back(ch);
                    }
                } else if (ch == '\n' || ch == -1) {
                    //error
                    t = new Token(ERR);
                } else {
                    str.push_back(ch);
                }
            }
            if (!t) {
                t = new Str(str);
            }
        }
        // numeric
        else if (ch >= '0' && ch <= '9') {
            int val = 0;
            if (ch != '0') {
                do {
                    val = val * 10 + ch - '0';
                    next();
                } while (ch >= '0' && ch <= '9');
            } else {
                next();
                if (ch == 'b') {
                    next();
                    if (ch >= '0' && ch <= '9') {
                        do {
                            val = val * 2 + ch - '0';
                            next();
                        } while (ch >= '0' && ch <= '9');
                    } else {
                        //error
                    }
                } else if (ch == 'x') {
                    next();
                    if (ch >= '0' && ch <= '9' || ch >= 'a' && ch <= 'f' || ch >= 'A' && ch <= 'F') {
                        do {
                            val = val * 16 + ch;
                            if (ch >= '0' && ch <= '9') {
                                val -= '0';
                            } else if (ch >= 'a' && ch <= 'f') {
                                val -= 'a';
                            } else { // equal to if (ch >= 'A' && ch <= 'F'){
                                val -= 'A';
                            }
                            next();
                        } while (ch >= '0' && ch <= '9' || ch >= 'a' && ch <= 'f' || ch >= 'A' && ch <= 'F');
                    } else {
                        //error
                    }
                } else if (ch >= '0' && ch <= '7') {
                    do {
                        val = val * 8 + ch - '0';
                        next();
                    } while (ch >= '0' && ch <= '7');
                }
            }
            if (!t) {
                // number
            }
        }
            //edge symbol
        else {
            switch (ch) {
                case '#':
                    // macro definition
                    while (!(ch == '\n' && ch == -1)) {
                        next();
                    }
                    // error
                    t = new Token(ERR);
                    break;
                case '+':
                    t = new Token(next('+') ? INC : ADD);
                    break;
                case '-':
                    t = new Token(next('-') ? DEC : SUB);
                    break;
                case '>':
                    t = new Token(next('=') ? GE : GT);
                    break;
                case '<':
                    t = new Token(next('=') ? LE : LT);
                    break;
                case '&':
                    t = new Token(next('&') ? AND : LEA);
                    break;
                case '=':
                    t = new Token(next('=') ? EQU : ASSIGN);
                    break;
                case '!':
                    t = new Token(next('=') ? NEQU : NOT);
                    break;
                case '/':
                    next();
                    if (ch == '/') { // single line comment
                        while (!(ch == '\n' || ch == -1)) {
                            next();
                        }
                    } else if (ch == '*') {
                        while (!next(-1)) {
                            if (ch == '*') {
                                while (next('*')) {
                                }
                                if (ch == '/') {
                                    break;
                                }

                            }
                        }
                        if (ch == -1) {
                            //error
                            t = new Token(ERR);
                        }

                    } else {
                        // DIV
                        t = new Token(DIV);
                    }
                    break;
                case '|':
                    t = new Token(next('|') ? OR : ERR);
                    break;
                case '*':
                    t = new Token(MUL);
                    next();
                    break;
                case '%':
                    t = new Token(MOD);
                    next();
                    break;
                case ',':
                    t = new Token(COMMA);
                    next();
                    break;
                case ':':
                    t = new Token(COLON);
                    next();
                    break;
                case ';':
                    t = new Token(SEMICON);
                    next();
                    break;
                case '(':
                    t = new Token(LPAREN);
                    next();
                    break;
                case ')':
                    t = new Token(RPAREN);
                    next();
                    break;
                case '[':
                    t = new Token(LBRACK);
                    next();
                    break;
                case ']':
                    t = new Token(RBRACK);
                    next();
                    break;
                case '{':
                    t = new Token(LBRACE);
                    next();
                    break;
                case '}':
                    t = new Token(RBRACE);
                    next();
                    break;
                case -1:
                    next();
                    break;
                default:
                    t = new Token(ERR);
                    next();
            }
        }
        delete token;
        token = t;
        if (token && token->tag != ERR) {
            return token;
        } else {
            continue;
        }
    }
    // end of file
    delete token;
    return token = new Token(END);
}

