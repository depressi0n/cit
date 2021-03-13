//
// Created by 欧阳世勇 on 2021/3/12.
//

#ifndef CIT_SYMTAB_H
#define CIT_SYMTAB_H

#include "lexer/token.h"
#include <unordered_map>
#include <vector>

using namespace std;

class Var {
public:
    static Var *getStep(Var *v);

    static Var *getVoid();

    static Var *getTrue();

    // a variable
    Var(vector<int> &sp, bool ext, Tag t, bool ptr, string name, Var *init = NULL);

    // an array
    Var(vector<int> &sp, bool ext, Tag t, string name, int len);

    // a literal
    Var(Token *lt);

    // int
    Var(int val);

    // temporary variable
    Var(vector<int> &sp, Tag t, bool ptr);

    // copy variable
    Var(vector<int> &sp, Var *v);

    // void变量
    Var();


    // initialize
    bool setInit();

    void setPointer(Var *p);

    void setLeft(bool lf);

    void setOffset(int off);

    bool isChar();

    bool isCharPtr();

    bool isVoid();

    bool isBase();

    bool isRef();

    bool isLiteral();

    Var *getInitData() const;

    const vector<int> &getScopePath() const;

    bool isExterned() const;

    bool getPtr();

    bool getArray();

    string getName();

    string getPtrVal();

    string getRawStr();

    Var *getPointer();

    string getStrVal();

    bool getLeft();

    int getOffset();

    int getSize();

    int getVal();

    void String();

    void clear();

    Tag getType() const;

private:
    bool externed;
    Tag type;
    string name;

    bool isPtr;
    string ptrVal;

    // can be used to initialize a defined variable
    bool literal;
    union {
        int intVal;
        char charVal;
    };
    string strVal;

    // pointer to  current variable
    Var *ptr;
    // if this variable is an array
    bool isArray;
    int arraySize;

    bool isLeft;

    bool inited;
    Var *initData;

    vector<int> scopePath;

    int size;
    int offset;

    void setExterned(bool externed);

    void setType(Tag t);

    void setName(const string &n);

    void setArray(int len);

    void setPtr(bool p);
};

class Func {
    bool externed;
    Tag type;
    string name;
    vector<Var *> paraVar;

    vector<int> scopeEsp;

public:
    Func(bool externed, Tag type, const string &name, const vector<Var *> &paraVar);

    ~Func();

    bool match(Func *func);

    bool match(vector<Var *> &args);

    // define a declared function
    void define(Func *def);

    void enterScope();

    void leaveScope();

    void locate(Var *var);

    bool isExterned() const;

    Tag getType() const;

    const vector<Var *> &getParaVar() const;

    const string &getName() const;

    void setExterned(bool ext);

    void String();

};


class SymTab {
public:
    SymTab();

    virtual ~SymTab();

    void enter();

    void leave();

    void addVar(Var *v);
    void addStr(Var *v);
    Var* getVar(string name);
    vector<Var*> getGlobalVars();

    void decFunc(Func *func);
    void defFunc(Func *func);
    void endDefFunc();

    Func* getFunc(string name,vector<Var*> &args);

    Func *getCurFun() const;

    vector<int> &getScopePath();

    void String();

private:
    vector<string> varList;
    vector<string> funcList;

    unordered_map<string, vector<Var *> *> varTab;
    unordered_map<string, Var *> strTab;
    unordered_map<string, Func *> funcTab;

    Func *curFun;
    int scopeId;
    vector<int> scopePath;

public:
    static Var *voidVar;
    static Var *zero;
    static Var *one;
    static Var *four;


};


#endif //CIT_SYMTAB_H
