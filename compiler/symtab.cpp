//
// Created by 欧阳世勇 on 2021/3/12.
//

#include "symtab.h"
#include "error.h"

Var::Var() {
    clear();
    setName("<void>");
    setLeft(false);
    intVal = 0;
    literal = false;
    type = KW_VOID;
    isPtr = true;
}

void Var::setExterned(bool ext) {
    Var::externed = ext;
    size = 0;
}

void Var::setType(Tag t) {
    Var::type = t;
    if (Var::type == KW_VOID) {
        SEMERROR(VOID_VAR, "");
        t = KW_INT;
    }
    if (!externed && t == KW_INT) {
        size = 4;
    }
    if (!externed && t == KW_CHAR) {
        size = 1;
    }
}

void Var::setName(const string &n) {
    if (n == "") {
        // generate a n
    }
    Var::name = n;
}

void Var::setArray(int len) {

    if (len <= 0) {
        SEMERROR(ARRAY_LEN_INVALID, name);
        return;
    }
    Var::isArray = true;
    Var::isLeft = false;
    Var::arraySize = len;
    if (!externed)len = len * len;
}

void Var::setPtr(bool p) {
    if (!p) return;
    Var::isPtr = true;
    if (!externed)size = 4;
}

bool Var::setInit() {
    // TODO check type
    Var *init = initData;
    if (!inited) return false;
    inited = false;
    if (externed) {
        SEMERROR(DEC_INIT_DENY, name);
    } else if (init->literal) {
        inited = true;
        // string
        if (init->isArray) {
            ptrVal = init->name;
        }
            // char  or int
        else {
            intVal = init->intVal;
        }
    } else {
        if (scopePath.size() == 1) {
            SEMERROR(GLB_INIT_ERR, name);
        } else {
            return true;
        }
    }
    return false;
}

Var *Var::getInitData() const {
    return initData;
}

const vector<int> &Var::getScopePath() const {
    return scopePath;
}

bool Var::isExterned() const {
    return externed;
}

Var *Var::getStep(Var *v) {
    if (v->isBase()) return SymTab::one;
    else if (v->type == KW_CHAR) return SymTab::one;
    else if (v->type == KW_INT) return SymTab::four;
    else return NULL;
}

Var *Var::getVoid() {
    return SymTab::voidVar;
}

Var *Var::getTrue() {
    return SymTab::one;
}

Var::Var(vector<int> &sp, Var *v) {
    clear();
    scopePath = sp;
    setType(v->type);
    setPtr(v->ptr);
    setName(v->name);
    setLeft(false);
}

Var::Var(vector<int> &sp, Tag t, bool ptr) {
    clear();
    scopePath = sp;
    setType(t);
    setPtr(ptr);
    setName("");
    setLeft(false);
}

Var::Var(int val) {
    clear();
    setName("<int>");
    literal = true;
    setLeft(false);
    setType(KW_INT);
    intVal = val;
}

// literal
Var::Var(Token *lt) {
    clear();
    literal = true;
    setLeft(false);
    switch (lt->tag) {
        case NUM:
            setType(KW_INT);
            name = "<int>";
            intVal = ((Num *) lt)->val;
            break;
        case CH:
            setType(KW_CHAR);
            name = "<char>";
            // set the high bits to zero
            intVal = 0;
            charVal = ((Char *) lt)->ch;
            break;
        case STR:
            setType(KW_CHAR);
            // TODO get a temporary name
            strVal = ((Str *) lt)->str;
            setArray(strVal.size() + 1);
            break;
    }
}

Var::Var(vector<int> &sp, bool ext, Tag t, string name, int len) {
    clear();
    scopePath = sp;
    setExterned(ext);
    setType(t);
    setName(name);
    setArray(len);
}

Var::Var(vector<int> &sp, bool ext, Tag t, bool ptr, string name, Var *init) {
    clear();
    scopePath = sp;
    setExterned(ext);
    setType(t);
    setPtr(ptr);
    setName(name);
    initData = init;
}

void Var::setPointer(Var *p) {
    ptr = p;
}

void Var::setLeft(bool lf) {
    isLeft = lf;
}

void Var::setOffset(int off) {
    offset = off;
}

bool Var::isChar() {
    return (type == KW_CHAR) && isBase();
}

bool Var::isCharPtr() {
    return (type == KW_CHAR) && !isBase();
}

bool Var::isBase() {
    return !isArray && !isPtr;
}

bool Var::isVoid() {
    return type == KW_VOID;
}

bool Var::isRef() {
    return !ptr;
}

bool Var::isLiteral() {
    return this->literal && isBase();
}

bool Var::getPtr() {
    return isPtr;
}

bool Var::getArray() {
    return isArray;
}

string Var::getName() {
    return name;
}

string Var::getPtrVal() {
    return ptrVal;
}

Var *Var::getPointer() {
    return ptr;
}

string Var::getStrVal() {
    return strVal;
}

string Var::getRawStr() {
    string raw;
    for (int i = 0; i < strVal.size(); ++i) {
        switch (strVal[i]) {
            case '\n':
                raw.append("\\n");
                break;
            case '\t':
                raw.append("\\t");
                break;
            case '\0':
                raw.append("\\000");
                break;
            case '\\':
                raw.append("\\\\");
                break;
            case '\"':
                raw.append("\\\"");
                break;
            default:
                raw.push_back(strVal[i]);
        }
    }
    raw.append("\\000");
    return raw;
}

bool Var::getLeft() {
    return isLeft;
}

int Var::getOffset() {
    return offset;
}

int Var::getSize() {
    return size;
}

void Var::String() {
    // extern
    if (externed)printf("extern ");
    // type
    printf("%s", tokenName[type]);
    // point
    if (isPtr) printf("*");
    // name
    printf(" %s", name.c_str());
    // array
    if (isArray) printf("[%d]", arraySize);
    // init data
    if (inited) {
        printf(" = ");
        switch (type) {
            case KW_INT:
                printf("%d", intVal);
                break;
            case KW_CHAR:
                if (isPtr) printf("<s>", ptrVal.c_str());
                else printf("%c", charVal);
                break;
        }
    }
    printf("; size<%d scope=\"", size);
    for (int i = 0; i < scopePath.size(); ++i) {
        printf("/%d", scopePath[i]);
    }
    printf("\" ");
    if (offset > 0) {
        printf("addr=[ebp+%d]", offset);
    } else if (offset < 0) {
        printf("addr=[ebp-%d]", offset);
    } else if (name[0] != '<') {
        printf("addr=<%s>", name.c_str());
    } else {
        printf("value ='%d'", getVal());
    }
}

int Var::getVal() {
    return intVal;
}

void Var::clear() {
    scopePath.push_back(-1);
    externed = false;
    isPtr = false;
    isArray = false;
    isLeft = true; //a variable can be default as left value
    inited = false;
    literal = false;

    size = 0;
    offset = 0;
    ptr = NULL;
    initData = NULL;
}

Tag Var::getType() const {
    return type;
}


// special variable
Var *SymTab::voidVar = NULL;
Var *SymTab::zero = NULL;
Var *SymTab::one = NULL;
Var *SymTab::four = NULL;

SymTab::SymTab() {
    voidVar = new Var();
    zero = new Var(1);
    one = new Var(1);
    four = new Var(4);

    scopeId = 0;
    curFun = NULL;
    scopePath.push_back(0);
}

SymTab::~SymTab() {
    unordered_map<string, Func *>::iterator funcIt, funcEnd = funcTab.end();
    for (funcIt = funcTab.begin(); funcIt != funcEnd; funcIt++) {
        delete funcIt->second;
    }
    unordered_map<string, vector<Var *> *>::iterator varIt, varEnd = varTab.end();
    for (varIt = varTab.begin(); varIt != varEnd; varIt++) {

        vector<Var *> &list = *varIt->second;
        for (int i = 0; i < list.size(); ++i) {
            delete list[i];
        }
        delete &list;
    }
    unordered_map<string, Var *>::iterator strIt, strEnd = strTab.end();
    for (strIt = strTab.begin(); strIt != strEnd; strIt++) {
        delete strIt->second;
    }
}

Func *SymTab::getCurFun() const {
    return curFun;
}

vector<int> &SymTab::getScopePath() {
    return scopePath;
}

void SymTab::enter() {
    scopeId++;
    scopePath.push_back(scopeId);
    if (curFun) curFun->enterScope();
}

void SymTab::leave() {
    scopePath.pop_back();
    if (curFun) curFun->leaveScope();
}

void SymTab::addVar(Var *v) {
    // do not exist the var list which have the same name
    if (varTab.find(v->getName()) == varTab.end()) {
        varTab[v->getName()] = new vector<Var *>;
        varTab[v->getName()]->push_back(v);
        varList.push_back(v->getName());
    } else {
        vector<Var *> &list = *varTab[v->getName()];
        int i;
        for (i = 0; i < list.size(); i++) {
            if (list[i]->getScopePath().back() == v->getScopePath().back()) {
                // ERROR have the same name
                break;
            }
        }
        if (i == list.size() || v->getName()[0] == '<') {
            list.push_back(v);
        } else {
            // ERROR
            delete v;
            return;
        }
    }
}

void SymTab::addStr(Var *v) {
    strTab[v->getName()] = v;
}
Var * SymTab::getVar(string name) {
    Var *select=NULL;
    if (varTab.find(name)!=varTab.end()){
        vector<Var*> &list=*varTab[name];
        int pathLen=scopePath.size();
        int maxLen=0;
        for (int i = 0; i < list.size(); ++i) {
            int len=list[i]->getScopePath().size();
            if (len<=pathLen && list[i]->getScopePath()[len-1] == scopePath[len-1]){
                if (len>maxLen){
                    maxLen=len;
                    select = list[i];
                }
            }
        }
    }
    if (!select){
        //ERROR undefined variable
    }
    return select;
}

vector<Var *> SymTab::getGlobalVars() {
    vector<Var *> glbVars;
    for (int i = 0; i < varList.size(); ++i) {
        string varName=varList[i];
        if (varName[0]=='<') continue;
        vector<Var*> &list=*varTab[varName];
        for (int j = 0; j < list.size(); ++j) {
            if (list[j]->getScopePath().size()==1 ){
                glbVars.push_back(list[j]);
                break;
            }
        }
    }
    return glbVars;
}

// declaration of function
void SymTab::decFunc(Func *func){
    func->setExterned(true);
    if (funcTab.find(func->getName())==funcTab.end()){
        funcTab[func->getName()]=func;
        funcList.push_back(func->getName());
    }else{
        Func *last=funcTab[func->getName()];
        if (!last->match(func)){
            //ERROR re-declaration
        }
        delete func;
    }
}

void SymTab::defFunc(Func *func) {
    if (func->isExterned()){
        //ERROR
        func->setExterned(false);
    }
    if (funcTab.find(func->getName())==funcTab.end()){
        funcTab[func->getName()]=func;
        funcList.push_back(func->getName());
    }else{
        Func *last=funcTab[func->getName()];
        if (last->isExterned()){
            if (!last->match(func)){
                //ERROR re-declaration
            }
            last->define(func);
        }else{
            //ERROR
        }
        delete func;
        func=last;
    }
    curFun=func;
}

void SymTab::endDefFunc() {
    // generate end of function
    // and then set NULL
    curFun=NULL;
}

Func *SymTab::getFunc(string name, vector<Var *> &args) {
    if (funcTab.find(name)!=funcTab.end()){
        Func *last=funcTab[name];
        if (!last->match(args)){
            //ERROR
            return NULL;
        }
        return last;
    }
    //ERROR
    return NULL;
}

void SymTab::String() {
    printf("------variable table------\n");
    for (int i = 0; i < varList.size(); ++i) {
        string varName=varList[i];
        vector<Var*>&list=*varTab[varName];
        printf("%s:\n",varName.c_str());
        for (int j = 0; j < list.size(); ++j) {
            printf("\t");
            list[j]->String();
            printf("\n");
        }
    }

    printf("------string table------\n");
    unordered_map<string,Var*>::iterator strIt,strEnd=strTab.end();
    for (strIt = strTab.begin();  strIt!=strEnd ; strIt++) {
        printf("%s=%s\n",strIt->second->getName().c_str(),strIt->second->getStrVal().c_str());
    }

    printf("------function table------\n");
    for (int i = 0; i < funcList.size(); ++i) {
        funcTab[funcList[i]]->String();
    }
}


bool Func::isExterned() const {
    return externed;
}

Tag Func::getType() const {
    return type;
}

const vector<Var *> &Func::getParaVar() const {
    return paraVar;
}

const string &Func::getName() const {
    return name;
}

void Func::setExterned(bool ext) {
    Func::externed = ext;
}

Func::Func(bool externed, Tag type, const string &name, const vector<Var *> &paraVar)
        : externed(externed), type(type), name(name), paraVar(paraVar) {

}

Func::~Func() {

}

bool Func::match(Func *func) {
    if (name != func->getName()) {
        return false;
    }
    if (paraVar.size() != func->getParaVar().size()) {
        return false;
    }
    int len = paraVar.size();
    for (int i = 0; i < len; ++i) {
        if (paraVar[i]->getType() != func->paraVar[i]->getType()) {
            // ERROR

        } else {
            return false;
        }
    }
    if (type != func->getType()) {
        // ERROR
    }
    return true;
}

bool Func::match(vector<Var *> &args) {
    if (paraVar.size() != args.size()) {
        return false;
    }
    int len = paraVar.size();
    for (int i = 0; i < len; ++i) {
        // check the type
        if (paraVar[i]->getType() != args[i]->getType()) {
            return false;
        }
    }
    return true;
}

void Func::define(Func *def) {
    externed = false;
    paraVar = def->getParaVar();

}

void Func::enterScope() {
    scopeEsp.push_back(0);
}

void Func::leaveScope() {
    scopeEsp.pop_back();
}

void Func::String() {
    // type
    printf("%s",tokenName[type]);
    // type name
    printf(" %s",name.c_str());
    // argument
    printf("(");
    for (int i = 0; i < paraVar.size(); ++i) {
        printf("<%s>",paraVar[i]->getName().c_str());
        if (i!=paraVar.size()-1) printf(",");
    }
    printf(")");
    if(externed) printf(";\n");
    else{
        printf(":\n");
    }
}





