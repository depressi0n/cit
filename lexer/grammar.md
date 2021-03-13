```
<program> -> <segment> <program> | ^  #一个程序包括多个片段

<segment> -> extern <segment'> | <segment'> # 一个片段内包括变量和函数的声明和定义，其中有extern的必然是声明
<segment'> -> <type> <def>
=>
<segment> -> extern <type> <def> | <type> <def>

<type> -> INT | CH | VOID 

# 支持多个变量
# 变量定义，要么是变量，数组，或者指针
# 函数定义和声明，都是以ID作为开始
<def> -> <defdata> <deflist> | <fun>
<defdata> -> ID <vardef> | MUL ID <init>
<deflist> -> , <defdata> <deflist> | ;
<vardef> -> [NUM] | <init>
<init> -> = <expr> | ^ #可以不初始化
<fun> -> ID ( <para> ) ; | ID ( <para> ) <block>  => <fun> -> ID ( <para> ) <funtail>  <funtail> -> ; | <block>
=>
<def> -> ID <vardef> <deflist> | MUL ID <init> <deflist> | ID ( <para> ) <funtail>
=>
<def> -> ID <idtail> | MUL ID <init> <deflist>
<idtail> -> <vardef> <deflist> | ( <para> ) <funtail>
<init> -> = <expr> | ^
<deflist> -> , <defdata> <deflist> | ;
<vardef> -> [NUM] | <init>
<funtail> -> ; | <block>
<defdata> -> ID <vardef> | MUL ID <init>


<para> -> <type> <paradata> <paralist> | ^
<paradata> -> MUL ID | ID | ID [NUM] | => <paradata> -> MUL ID | ID <paradatatail>  <paradatatail> -> [NUM] | ^
<paralist> -> , <type><paradata> <paralist> | ^
=>
<para> -> <type> <paradata> <paralist> | ^
<paradata> -> MUL ID | ID <paradatatail>
<paralist> -> ,<type> <paradata> <paralist> | ^
<paradatatail> -> [NUM] | ^


<block> -> { <subprogram> }
<subprogram> -> <localdef> <subprogram> | <statement> <subprogram> | ^
<localdef> -> <type> <defdata> <deflist>

# 高优先级的结果作为低优先级的操作数
<expr> -> <assexpr>

<assexpr> -> <orexpr> <asstail>
<asstail> -> = <orexpr> <asstail> | ^

<orexpr> -> <andexpr> <ortail>
<ortail> -> OR <andexpr> <ortail> | ^

<andexpr> -> <cmpexpr> <andtail>
<andtail> -> AND <cmpexpr> <andtai> | ^

<cmpexpr> -> <aloexpr> <cmptail> 
<cmptail> -> <cmps> <aloexpr> <cmptail> | ^
<cmps> -> GT | GE | LT | LE | EQU | NEQU

<aloexpr> -> <item> <alotail>
<alotail> -> <adds> <item> <alotail> | ^
<adds> -> ADD | SUB

<item> -> <factor> <itemtail> 
<itemtail> -> <muls> <factors> <itemtail> | ^
<muls> -> MUL | DIV | MOD

<factor> -> <lop> <factor> | <val>
<lop> -> NOT | SUB | LEA | MUL | INCR | DECR

<val> -> <elem> <rop>
<rop> -> INC | DEC

<elem> -> ID | ID [ <expr> ] | ID ( <realarg> ) | ( <expr> ) | <literal>
=>
<elem> -> ID <idexpr> | ( <expr> ) | <literal> 
<idexpr> -> [ <expr> ] | ( <realarg> ) | ^
<realarg> -> <arg> <arglist> | ^
<arglist> -> , <arg> <arglist> | ^
<arg> -> <expr>

<literal> -> NUM | CH | STR
<altexpr> -> <expr> | ^

# 语句包括表达式语句；while、do-while、for循环语句；if-else、switch-case分支语句；break、continue、return语句
<statement> -> <altexpr> ; | <whilestat> | <forstat> | <dowhilestat> | <ifstat> | <switchstat> | BREAK | CONTINUE |RETURN ; | RETURN <altexpr> ;
<whilestat> -> WHILE ( <altexpr> )  <block>
<dowhilestat> -> DO <block> WHILE ( <altexpr> ) ; 
<forstat> -> FOR ( <forinit> <altexpr> ; <altexpr> ) <block>
<forinit> -> <localdef> | <altexpr> ;
<ifstat> -> IF ( <expr> ) <block> <elsestat>
<elsestat> -> ELSE <block> | ^
<switchstat> -> SWITCH ( <expr> ) { <casestat> }
<casestat> -> CASE <caselabel> :  <subprogram> <casestat> | DEFAULT : <subprogram>
<caselabel> -> <literal>

```

```
<program> -> <segment> <program> | ^  #一个程序包括多个片段
<segment> -> extern <type> <def> | <type> <def>
<type> -> INT | CHAR | VOID 
<def> -> ID <idtail> | MUL ID <init> <deflist>
<idtail> -> <vardef> <deflist> | ( <para> ) <funtail>
<init> -> = <expr> | ^
<deflist> -> , <defdata> <deflist> | ;
<vardef> -> [NUM] | <init>
<funtail> -> ; | <block>
<defdata> -> ID <vardef> | MUL ID <init>
<para> -> <type> <paradata> <paralist> | ^
<paradata> -> MUL ID | ID <paradatatail>
<paralist> -> ,<type> <paradata> <paralist> | ^
<paradatatail> -> [NUM] | ^
<block> -> { <subprogram> }
<subprogram> -> <localdef> <subprogram> | <statement> <subprogram> | ^
<localdef> -> <type> <defdata> <deflist>
<expr> -> <assexpr>
<assexpr> -> <orexpr> <asstail>
<asstail> -> = <orexpr> <asstail> | ^
<orexpr> -> <andexpr> <ortail>
<ortail> -> OR <andexpr> <ortail> | ^
<andexpr> -> <cmpexpr> <andtail>
<andtail> -> AND <cmpexpr> <andtai> | ^
<cmpexpr> -> <aloexpr> <cmptail> 
<cmptail> -> <cmps> <aloexpr> <cmptail> | ^
<cmps> -> GT | GE | LT | LE | EQU | NEQU
<aloexpr> -> <item> <alotail>
<alotail> -> <adds> <item> <alotail> | ^
<adds> -> ADD | SUB
<item> -> <factor> <itemtail> 
<itemtail> -> <muls> <factor> <itemtail> | ^
<muls> -> MUL | DIV | MOD
<factor> -> <lop> <factor> | <val>
<lop> -> NOT | SUB | LEA | MUL | INCR | DECR
<val> -> <elem> <rop>
<rop> -> INC | DEC
<elem> -> ID <idexpr> | ( <expr> ) | <literal> 
<idexpr> -> [ <expr> ] | ( <realarg> ) | ^
<realarg> -> <arg> <arglist> | ^
<arglist> -> , <arg> <arglist> | ^
<arg> -> <expr>
<literal> -> NUM | CH | STR
<altexpr> -> <expr> | ^
<statement> -> <altexpr> ; | <whilestat> | <forstat> | <dowhilestat> | <ifstat> | <switchstat> | BREAK | CONTINUE |RETURN ; | RETURN <altexpr> ;
<whilestat> -> WHILE ( <altexpr> )  <block>
<dowhilestat> -> DO <block> WHILE ( <altexpr> ) ; 
<forstat> -> FOR ( <forinit> <altexpr> ; <altexpr> ) <block>
<forinit> -> <localdef> | <altexpr> ;
<ifstat> -> IF ( <expr> ) <block> <elsestat>
<elsestat> -> ELSE <block> | ^
<switchstat> -> SWITCH ( <expr> ) { <casestat> }
<casestat> -> CASE <caselabel> :  <subprogram> <casestat> | DEFAULT : <subprogram>
<caselabel> -> <literal>
```

FIRST SET
```
FIRST(<program>) = extern | TYPE_FIRST | ^ 
FIRST(<segment>) = extern | TYPE_FIRST
FIRST(<type>) = INT | CHAR | VOID 
FIRST(<def>) = ID | MUL 
FIRST(<idtail>) = [ | = | , | ; | ( 
FIRST(<init>) = = | ^
FIRST(<deflist>) = , | ;
FIRST(<vardef>) = [ | = | ^
FIRST(<funtail>) = ; | {
FIRST(<defdata>) = ID | MUL
FIRST(<para>) = TYPE_FIRST | ^
FIRST(<paradata>) = MUL | ID 
FIRST(<paralist>) = , | ^
FIRST(<paradatatail>) = [ | ^
FIRST(<block>) = {
FIRST(<subprogram>) = TYPE_FIRST | STATEMENT_FISRT | ^
FIRST(<localdef>) = TYPE_FIRST 
FIRST(<expr>) = NOT | SUB | LEA | MUL | INCR | DECR | ID | ( | NUM | CH | STR
FIRST(<assexpr>) = EXPR_FIRST
FIRST(<asstail>) = = | ^
FIRST(<orexpr>) = EXPR_FIRST
FIRST(<ortail>) = OR | ^
FIRST(<andexpr>) = EXPR_FIRST
FIRST(<andtail>) = AND | ^
FIRST(<cmpexpr>) = EXPR_FIRST
FIRST(<cmptail>) = GT | GE | LT | LE | EQU | NEQU | ^
FIRST(<cmps>) = GT | GE | LT | LE | EQU | NEQU
FIRST(<aloexpr>) = EXPR_FIRST
FIRST(<alotail>) = ADD | SUB | ^
FIRST(<adds>) = ADD | SUB
FIRST(<item>) = EXPR_FIRST
FIRST(<itemtail>) = MUL | DIV | MOD | ^
FIRST(<muls>) = MUL | DIV | MOD
FIRST(<factor>) = EXPR_FIRST
FIRST(<lop>) = NOT | SUB | LEA | MUL | INCR | DECR
FIRST(<val>) = ID | ( | NUM | CH | STR
FIRST(<rop>) = INC | DEC
FIRST(<elem>) = ID | ( | NUM | CH | STR
FIRST(<idexpr>) = [ | ( | ^
FIRST(<realarg>) = EXPR_FIRST | ^
FIRST(<arglist>) = , | ^
FIRST(<arg>) = EXPR_FIRST
FIRST(<literal>) = NUM | CH | STR
FIRST(<altexpr>) = EXPR_FIRST | ^
FIRST(<statement>) = EXPR_FIRST | ; | WHILE | FOR | DO | IF | SWITCH | BREAK | CONTINUE | RETURN
FIRST(<whilestat>) = WHILE 
FIRST(<dowhilestat>) = DO 
FIRST(<forstat>) = FOR 
FIRST(<forinit>) = TYPE_FIRST | EXPR_FIRST | ;
FIRST(<ifstat>) = IF
FIRST(<elsestat>) = ELSE | ^
FIRST(<switchstat>) = SWITCH 
FIRST(<casestat>) = CASE | DEFAULT 
FIRST(<caselabel>) = NUM | CH | STR
```
WRONG FIRST SET
```
FIRST(<program>) = extern | TYPE_FIRST  | ^
FIRST(<segment>) = extern | TYPE_FIRST 
FIRST(<type>) = INT | CHAR | VOID = TYPE_FIRST
FIRST(<def>) = ID | MUL
FIRST(<idtail>) = [ | EXPR_FIRST | , | ; | ( 
FIRST(<init>) = = | ^
FIRST(<deflist>) = , | ;
FIRST(<vardef>) = [ | EXPR_FIRST | ^
FIRST(<funtail>) = ; | {
FIRST(<defdata>) = ID | MUL
FIRST(<para>) = TYPE_FIRST | ^
FIRST(<paradata>) = MUL | ID
FIRST(<paralist>) = , | ^
FIRST(<paradatatail>) = [ | ^
FIRST(<block>) = { 
FIRST(<subprogram>) = TYPE_FIRST  | STATEMENT_FIRST | ^
FIRST(<localdef>) = TYPE_FIRST
FIRST(<expr>) = NOT | SUB | LEA | MUL | INCR | DECR | ID | ( | NUM | CH | STR
FIRST(<assexpr>) = EXPR_FIRST
FIRST(<asstail>) = = | ^
FIRST(<orexpr>) = EXPR_FIRST
FIRST(<ortail>) = OR | ^
FIRST(<andexpr>) = EXPR_FIRST
FIRST(<andtail>) = AND | ^
FIRST(<cmpexpr>) = EXPR_FIRST
FIRST(<cmptail>) = GT | GE | LT | LE | EQU | NEQU | ^
FIRST(<cmps>) = GT | GE | LT | LE | EQU | NEQU
FIRST(<aloexpr>) = EXPR_FIRST
FIRST(<alotail>) = ADD | SUB | ^
FIRST(<adds>) = ADD | SUB
FIRST(<item>) = EXPR_FIRST
FIRST(<itemtail>) = MUL | DIV | MOD | ^
FIRST(<muls>) = MUL | DIV | MOD
FIRST(<factor>) = EXPR_FIRST
FIRST(<lop>) = NOT | SUB | LEA | MUL | INCR | DECR
FIRST(<val>) = ID | ( | NUM | CH | STR
FIRST(<rop>) = INC | DEC
FIRST(<elem>) = ID | ( | NUM | CH | STR
FIRST(<idexpr>) = [ | ( | ^
FIRST(<realarg>) = <arg> <arglist> | ^
FIRST(<arglist>) = , | ^
FIRST(<arg>) = EXPR_FIRST
FIRST(<literal>) = NUM | CH | STR
FIRST(<altexpr>) = EXPR_FIRST | ^
FIRST(<statement>) = EXPR_FIRST | ; | WHILE | FOR | DO | IF | SWITCH | BREAK | CONTINUE | RETURN
FIRST(<whilestat>) = WHILE
FIRST(<dowhilestat>) = DO
FIRST(<forstat>) = FOR
FIRST(<forinit>) = TYPE_FIRST
FIRST(<ifstat>) = IF 
FIRST(<elsestat>) = ELSE | ^
FIRST(<switchstat>) = SWITCH 
FIRST(<casestat>) = CASE  | DEFAULT 
FIRST(<caselabel>) = NUM | CH | STR
```

FOLLOW SET
```
FOLLOW(<program>) = #
FOLLOW(<segment>) = FIRST(<program>) U FOLLOW(<program>) = extern | TYPE_FIRST | #
FOLLOW(<type>) = FIRST(<def>) U FIRST(<paradata>) U FIRST(<defdata>) = ID | MUL 
FOLLOW(<def>) = FOLLOW(<segment>) = extern | TYPE_FIRST | #
FOLLOW(<idtail>) = FOLLOW(<def>) = extern | TYPE_FIRST | #
FOLLOW(<init>) = = FIRST(<deflist>) U FOLLOW(<vardef>) U FOLLOW(<defdata>) = , | ; | extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST
FOLLOW(<deflist>) = FOLLOW(<def>) U FOLLOW(<idtail>) U FOLLOW(<localdef>) = extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST | ;
FOLLOW(<vardef>) = FIRST(<deflist>) U FOLLOW(<defdata>) =  , | ; | extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST 
FOLLOW(<funtail>) = FOLLOW(<idtail>) = extern | TYPE_FIRST | #
FOLLOW(<defdata>) = FOLLOW(<deflist>) = extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST | ;
FOLLOW(<para>) = )
FOLLOW(<paradata>) = FIRST(<paralist>) U FOLLOW(<para>) U FOLLOW(<paralist>) = , | ) 
FOLLOW(<paralist>) = FOLLOW(<para>) = )
FOLLOW(<paradatatail>) = FOLLOW(<paradata>) = , | ) 
FOLLOW(<block>) = FOLLOW(<funtail>) = extern | TYPE_FIRST | #
FOLLOW(<subprogram>) = } 
FOLLOW(<localdef>) = FOLLOW(<subprogram>) U FIRST(<subprogram>) U FOLLOW(<forinit>) = } | TYPE_FIRST | STATEMENT_FISRT | EXPR_FIRST | ;
FOLLOW(<expr>) = FOLLOW(<init>) U ) U ] U FOLLOW(<arg>) U FOLLOW(<altexpr>) =  , | ; | extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST | ) | }
FOLLOW(<assexpr>) = FOLLOW(<expr>) = , | ; | extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST | ) | } 
FOLLOW(<asstail>) = FOLLOW(<assexpr>) = , | ; | extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST | ) | }
FOLLOW(<orexpr>) = FIRST(<asstail>) U FOLLOW(<asstail>) = = | , | ; | extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST | ) | }
FOLLOW(<ortail>) = FOLLOW(<orexpr>) = = | , | ; | extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST | ) | }
FOLLOW(<andexpr>) = FIRST(<ortail>) U FOLLOW(<ortail>) = OR | = | , | ; | extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST | ) | }
FOLLOW(<andtail>) = FOLLOW(<andexpr>) = = | OR |, | ; | extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST | ) | }
FOLLOW(<cmpexpr>) =FIRST(<andtail>) U FOLLOW(<andtail>) = AND | = | OR | , | ; | extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST | ) | }
FOLLOW(<cmptail>) = FOLLOW(<cmpexpr>) = AND | = | OR | , | ; | extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST | ) | }
FOLLOW(<cmps>) = FIRST(<aloexpr>) = EXPR_FIRST
FOLLOW(<aloexpr>) = FIRST(<cmptail>) U FOLLOW(<cmptail>) =  GT | GE | LT | LE | EQU | NEQU | AND | = | OR | , | ; | extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST | ) | }
FOLLOW(<alotail>) = FOLLOW(<aloexpr>) = GT | GE | LT | LE | EQU | NEQU | AND | = | OR | , | ; | extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST | ) | }
FOLLOW(<adds>) = FIRST(<item>) = EXPR_FIRST
FOLLOW(<item>) = FIRST(<alotail>) U FOLLOW(<aloexpr>) U FOLLOW(<alotail>) = ADD | SUB | GT | GE | LT | LE | EQU | NEQU | AND | = | OR | , | ; | extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST | ) | }
FOLLOW(<itemtail>) = FOLLOW(<item>) = ADD | SUB | GT | GE | LT | LE | EQU | NEQU | AND | = | OR | , | ; | extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST | ) | }
FOLLOW(<muls>) = FIRST(<factor>) = EXPR_FIRST
FOLLOW(<factor>) = FIRST(<itemtail>) U FOLLOW(<item>) U FOLLOW(<itemtail>) = MUL | DIV | MOD | ADD | SUB | GT | GE | LT | LE | EQU | NEQU | AND | = | OR | , | ; | extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST | ) | }
FOLLOW(<lop>) = FIRST(<factor>) = EXPR_FIRST
FOLLOW(<val>) = FOLLOW(<factor>) = MUL | DIV | MOD | ADD | SUB | GT | GE | LT | LE | EQU | NEQU | AND | = | OR | , | ; | extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST | ) | }
FOLLOW(<rop>) = FOLLOW(<val>) = MUL | DIV | MOD | ADD | SUB | GT | GE | LT | LE | EQU | NEQU | AND | = | OR | , | ; | extern | TYPE_FIRST | # | } | STATEMENT_FISRT | EXPR_FIRST | ) | }
FOLLOW(<elem>) = FIRST(<rop>) = INC | DEC
FOLLOW(<idexpr>) = FOLLOW(<elem>) = INC | DEC
FOLLOW(<realarg>) = )
FOLLOW(<arglist>) = FOLLOW(<realarg>) = )
FOLLOW(<arg>) = FIRST(<arglist>) U FOLLOW(<arglist>) U FOLLOW(<realarg>) = , | ) 
FOLLOW(<literal>) =  FOLLOW(<elem>) U FOLLOW(<caselabel>) = INC | DEC | :
FOLLOW(<altexpr>) = ; | )
FOLLOW(<statement>) = FIRST(<subprogram>) U FOLLOW(<subprogram>) =TYPE_FIRST | STATEMENT_FISRT | } 
FOLLOW(<whilestat>) = FOLLOW(<statement>) = TYPE_FIRST | STATEMENT_FISRT | } 
FOLLOW(<dowhilestat>) = FOLLOW(<statement>) = TYPE_FIRST | STATEMENT_FISRT | }  
FOLLOW(<forstat>) = FOLLOW(<statement>) = TYPE_FIRST | STATEMENT_FISRT | } 
FOLLOW(<forinit>) = FIRST(<altexpr>) U ; =  EXPR_FIRST | ;
FOLLOW(<ifstat>) = FOLLOW(<statement>) = TYPE_FIRST | STATEMENT_FISRT | } 
FOLLOW(<elsestat>) = FOLLOW(<ifstat>) = TYPE_FIRST | STATEMENT_FISRT | } 
FOLLOW(<switchstat>) = FOLLOW(<statement>) = TYPE_FIRST | STATEMENT_FISRT | } 
FOLLOW(<casestat>) = }
FOLLOW(<caselabel>) = :
```


WRONG FOLLOW SET
```
FOLLOW(<program>) = #
FOLLOW(<segment>) = FIRST(<program>) 
FOLLOW(<type>) = FIRST(<def>) U FIRST(<paradata>) U FIRST(<defdata>)
FOLLOW(<def>) = FOLLOW(<segment>) 
FOLLOW(<idtail>) = FOLLOW(<def>)
FOLLOW(<init>) = = FIRST(<deflist>) U FOLLOW(<vardef>) U FOLLOW(<defdata>)
FOLLOW(<deflist>) = FOLLOW(<def>) U FOLLOW(<idtail>) U FOLLOW(<localdef>)
FOLLOW(<vardef>) = FIRST(<deflist>) U FOLLOW(<defdta>)
FOLLOW(<funtail>) = FOLLOW(<idtail>)
FOLLOW(<defdata>) = FIRST(<deflist>)
FOLLOW(<para>) = )
FOLLOW(<paradata>) = FIRST(<paralist>)
FOLLOW(<paralist>) = FOLLOW(<para>)
FOLLOW(<paradatatail>) = FOLLOW(<paradata>)
FOLLOW(<block>) = FOLLOW(<funtail>)
FOLLOW(<subprogram>) = }
FOLLOW(<localdef>) = FIRST(<subprogram>) U FOLLOW(<subprogram>)
FOLLOW(<expr>) = FOLLOW(<init>) U ) U ] U FOLLOW(<arg>) U FOLLOW(<altexpr>)
FOLLOW(<assexpr>) = FOLLOW(<expr>)
FOLLOW(<asstail>) = FOLLOW(<assexpr>)
FOLLOW(<orexpr>) = FIRST(<asstail>)
FOLLOW(<ortail>) = FOLLOW(<orexpr>)
FOLLOW(<andexpr>) = FIRST(<ortails>)
FOLLOW(<andtail>) = FOLLOW(<andexpr>)
FOLLOW(<cmpexpr>) = FIRST(<andtail>)
FOLLOW(<cmptail>) = FOLLOW(<cmpexp>)
FOLLOW(<cmps>) = FIRST(<aloexpr>)
FOLLOW(<aloexpr>) = FIRST(<cmptail>)
FOLLOW(<alotail>) = FOLLOW(<aloexpr>)
FOLLOW(<adds>) = FIRST(<item>)
FOLLOW(<item>) = FIRST(<alotail>)
FOLLOW(<itemtail>) = FOLLOW(<item>)
FOLLOW(<muls>) = FIRST(<factors>)
FOLLOW(<factor>) = FIRST(<itemtail>)
FOLLOW(<lop>) = FIRST(<factor>)
FOLLOW(<val>) = FOLLOW(<factor>)
FOLLOW(<rop>) = FOLLOW(<val>)
FOLLOW(<elem>) = FIRST(<rop>)
FOLLOW(<idexpr>) = FOLLOW(<elem>)
FOLLOW(<realarg>) = )
FOLLOW(<arglist>) = )
FOLLOW(<arg>) = FIRST(<arglist>)
FOLLOW(<literal>) = FOLLOW(<elem>) U FOLLOW(<caselabel>)
FOLLOW(<altexpr>) = ; U ) 
FOLLOW(<statement>) = FIRST(<subprogram>)
FOLLOW(<whilestat>) = FOLLOW(<statement>)
FOLLOW(<dowhilestat>) = FOLLOW(<statement>)
FOLLOW(<forstat>) = FOLLOW(<statement>)
FOLLOW(<forinit>) = FIRST(<altexpr>)
FOLLOW(<ifstat>) = FOLLOW(<statement>)
FOLLOW(<elsestat>) = FOLLOW(<ifstat>)
FOLLOW(<switchstat>) = FOLLOW(<statement>)
FOLLOW(<casestat>) = }
FOLLOW(<caselabel>) = :
```
```
FOLLOW(<program>) = #
FOLLOW(<segment>) = extern | TYPE_FIRST
FOLLOW(<type>) = ID | MUL 
FOLLOW(<def>) = extern | TYPE_FIRST
FOLLOW(<idtail>) = extern | TYPE_FIRST
FOLLOW(<init>) = = , | ; 
FOLLOW(<deflist>) = extern | TYPE_FIRST | STATEMENT_FIRST
FOLLOW(<vardef>) = , | ; 
FOLLOW(<funtail>) = extern | TYPE_FIRST
FOLLOW(<defdata>) = , | ;
FOLLOW(<para>) = )
FOLLOW(<paradata>) = , 
FOLLOW(<paralist>) = )
FOLLOW(<paradatatail>) = ,
FOLLOW(<block>) = extern | TYPE_FIRST
FOLLOW(<subprogram>) = }
FOLLOW(<localdef>) = TYPE_FIRST  | STATEMENT_FIRST | }
FOLLOW(<expr>) = , | ; |  ) | ] 
FOLLOW(<assexpr>) = , | ; |  ) | ] 
FOLLOW(<asstail>) = , | ; |  ) | ] 
FOLLOW(<orexpr>) = =
FOLLOW(<ortail>) = =
FOLLOW(<andexpr>) = OR
FOLLOW(<andtail>) = OR
FOLLOW(<cmpexpr>) = AND
FOLLOW(<cmptail>) = AND
FOLLOW(<cmps>) = EXPR_FIRST
FOLLOW(<aloexpr>) = GT | GE | LT | LE | EQU | NEQU
FOLLOW(<alotail>) = GT | GE | LT | LE | EQU | NEQU
FOLLOW(<adds>) = EXPR_FIRST
FOLLOW(<item>) = ADD | SUB
FOLLOW(<itemtail>) = ADD | SUB
FOLLOW(<muls>) = EXPR_FIRST
FOLLOW(<factor>) = MUL | DIV | MOD
FOLLOW(<lop>) = EXPR_FIRST
FOLLOW(<val>) = MUL | DIV | MOD
FOLLOW(<rop>) = MUL | DIV | MOD
FOLLOW(<elem>) = NOT | SUB | LEA | MUL | INCR | DECR
FOLLOW(<idexpr>) = NOT | SUB | LEA | MUL | INCR | DECR
FOLLOW(<realarg>) = )
FOLLOW(<arglist>) = )
FOLLOW(<arg>) = ,
FOLLOW(<literal>) = NOT | SUB | LEA | MUL | INCR | DECR | :
FOLLOW(<altexpr>) = ; | ) 
FOLLOW(<statement>) = TYPE_FIRST  | STATEMENT_FIRST
FOLLOW(<whilestat>) = TYPE_FIRST  | STATEMENT_FIRST
FOLLOW(<dowhilestat>) = TYPE_FIRST  | STATEMENT_FIRST
FOLLOW(<forstat>) = TYPE_FIRST  | STATEMENT_FIRST
FOLLOW(<forinit>) = EXPR_FIRST
FOLLOW(<ifstat>) = TYPE_FIRST  | STATEMENT_FIRST
FOLLOW(<elsestat>) = TYPE_FIRST  | STATEMENT_FIRST
FOLLOW(<switchstat>) = TYPE_FIRST  | STATEMENT_FIRST
FOLLOW(<casestat>) = }
FOLLOW(<caselabel>) = :
```

分析并找出对函数或者变量下定义或者声明的产生式
```
<program> -> <segment> <program> | ^  void program()

<segment> -> extern <segment'> | <segment'> # 一个片段内包括变量和函数的声明和定义，其中有extern的必然是声明
<segment'> -> <type> <def>
=>
<segment> -> extern <type> <def> | <type> <def> // void segment() 将extern和Tag都传入def

<type> -> INT | CH | VOID  // Tag type() 返回值为Tag，明确类型

# 支持多个变量
# 变量定义，要么是变量，数组，或者指针
# 函数定义和声明，都是以ID作为开始
<def> -> <defdata> <deflist> | <fun>       
<defdata> -> ID <vardef> | MUL ID <init>  // 获得指针或者变量名，并将其传入vardef和init，并返回一个变量 
<deflist> -> , <defdata> <deflist> | ;
<vardef> -> [NUM] | <init>
<init> -> = <expr> | ^ #可以不初始化
<fun> -> ID ( <para> ) ; | ID ( <para> ) <block>  => <fun> -> ID ( <para> ) <funtail>  <funtail> -> ; | <block>
=>
<def> -> ID <vardef> <deflist> | MUL ID <init> <deflist> | ID ( <para> ) <funtail>
=>
<def> -> ID <idtail> | MUL ID <init> <deflist> // void def(bool ext,Tag t) 将ext,t和name传给idtail 和 ext,t和name,isPtr传给init
<idtail> -> <vardef> <deflist> | ( <para> ) <funtail> // void idtail(bool ext,Tag bool isPtr,string name) 在函数内部完成函数和变量的创建，并将创建的函数func传入funtail对其完成声明或定义的工作
<init> -> = <expr> | ^ // Var init(bool ext,Tag t,bool isPtr,string name) 结合vardef得出要传isPtr参数
<deflist> -> , <defdata> <deflist> | ;  // void deflist(bool ext,Tag t) 传入ext和t的目的是处理遇到分号的情况
<vardef> -> [NUM] | <init> //Var vardef(bool ext,Tag t,bool isPtr,string name)
<funtail> -> ; | <block> // void funtail(Func *func) 创建函数，将其传入block中确定函数是声明还是定义
<defdata> -> ID <vardef> | MUL ID <init>  // Var defdata(bool ext,Tag t)


<para> -> <type> <paradata> <paralist> | ^  
    <paradata> -> MUL ID | ID | ID [NUM]  //完成变量，数组，指针的参数声明 
    <paralist> -> , <type><paradata> <paralist> | ^
    => 
    <paradata> -> MUL ID | ID <paradatatail>  
    <paradatatail> -> [NUM] | ^ 
=>
<para> -> <type> <paradata> <paralist> | ^ // void para(vector<Var*> &arg) arg用于接收参数列表,将Tag传入paradata
<paradata> -> MUL ID | ID <paradatatail> // Var paradata(Tag t) 将tag和name传入paradatatail完成变量的创建
<paralist> -> , <type> <paradata> <paralist> | ^ // void paralist(vector<Var*> &arg) 接收后续的参数
<paradatatail> -> [NUM] | ^ // Var paradatatail(Tag t,string name)


<block> -> { <subprogram> } // void block(Func func*) 完成函数定义，如果没有传入则使用curFun来完成定义工作 void block()
<subprogram> -> <localdef> <subprogram> | <statement> <subprogram> | ^  // void subprogram()注意作用域变化
<localdef> -> <type> <defdata> <deflist> // void localdef() 要注意作用域变化，完成变量声明、添加等工作

# 高优先级的结果作为低优先级的操作数
<expr> -> <assexpr> //Var expr() 返回表达式的结果 

<assexpr> -> <orexpr> <asstail> // Var assexpr()
<asstail> -> = <orexpr> <asstail> | ^ //  Var asstail(Var val) 赋值表达式是有结果值,完成赋值操作，返回给上一层

<orexpr> -> <andexpr> <ortail> //Var orexpr()
<ortail> -> OR <andexpr> <ortail> | ^ // Var ortail(Var lval) 完成计算

<andexpr> -> <cmpexpr> <andtail> // Var andexpr()
<andtail> -> AND <cmpexpr> <andtai> | ^  // Var andtail(Var lval) 完成计算

<cmpexpr> -> <aloexpr> <cmptail>  // Var cmpexpr()
<cmptail> -> <cmps> <aloexpr> <cmptail> | ^ //Var cmptail(Var lval) 完成计算
<cmps> -> GT | GE | LT | LE | EQU | NEQU //Tag cmps()

<aloexpr> -> <item> <alotail> // Var aloexpr()
<alotail> -> <adds> <item> <alotail> | ^ // Var alotail(Var lval)
<adds> -> ADD | SUB // Tag adds()

<item> -> <factor> <itemtail> // Var item()
<itemtail> -> <muls> <factors> <itemtail> | ^ // Var itemtail(Var lval)
<muls> -> MUL | DIV | MOD //Tag muls()

<factor> -> <lop> <factor> | <val>  //  Var factor()
<lop> -> NOT | SUB | LEA | MUL | INCR | DECR //Tag lop()

<val> -> <elem> <rop> //Var val() 返回一个变量或者常量用于调用函数的计算
<rop> -> INC | DEC //Tag rop()++或者--

<elem> -> ID | ID [ <expr> ] | ID ( <realarg> ) | ( <expr> ) | <literal>
=>
<elem> -> ID <idexpr> | ( <expr> ) | <literal>   //Var elem()变量、数组、函数调用参数、常量等，将名字传给idexpr
<idexpr> -> [ <expr> ] | ( <realarg> ) | ^ // Var idexpr(string name)
<realarg> -> <arg> <arglist> | ^ // void realarg(vector<Var> &args)
<arglist> -> , <arg> <arglist> | ^ // void arglist(vector<Var> &args)
<arg> -> <expr> // Var arg()

<literal> -> NUM | CH | STR // Var literal()
<altexpr> -> <expr> | ^ //Var altexpr()

# 语句包括表达式语句；while、do-while、for循环语句；if-else、switch-case分支语句；break、continue、return语句
<statement> -> <altexpr> ; | <whilestat> | <forstat> | <dowhilestat> | <ifstat> | <switchstat> | BREAK | CONTINUE |RETURN ; | RETURN <altexpr> ; // void statement()注意作用域
<whilestat> -> WHILE ( <altexpr> )  <block> // void whilestat() 注意作用域
<dowhilestat> -> DO <block> WHILE ( <altexpr> ) ; // void dowhilestat() 注意作用域
<forstat> -> FOR ( <forinit> <altexpr> ; <altexpr> ) <block> // void forstat() 注意作用域
<forinit> -> <localdef> | <altexpr> ; // void forinit() 注意作用域
<ifstat> -> IF ( <expr> ) <block> <elsestat> // void ifstat() 注意作用域
<elsestat> -> ELSE <block> | ^ // void elsestat() 注意作用域
<switchstat> -> SWITCH ( <expr> ) { <casestat> } // void switch() 注意作用域,并将表达式的值传递给casestat
<casestat> -> CASE <caselabel> :  <subprogram> <casestat> | DEFAULT : <subprogram> // void casestat(Var cond)
<caselabel> -> <literal> // Var caselabel()

```

```
<program> -> <segment> <program> | ^  #一个程序包括多个片段
<segment> -> extern <type> <def> | <type> <def>
<type> -> INT | CHAR | VOID  // 确定类型，返回值为Tag
<def> -> ID <idtail> | MUL ID <init> <deflist> //确定变量或函数名，需要对符号表中变量或者函数进行管理
<idtail> -> <vardef> <deflist> | ( <para> ) <funtail>  // 确定一组变量，或者确定一个函数，需要在内部进行管理
<init> -> = <expr> | ^ //完成变量的初始化工作，返回值为变量
<deflist> -> , <defdata> <deflist> | ;  // 确定一组变量
<vardef> -> [NUM] | <init> // 数组或者变量的初始化,返回值为变量
<funtail> -> ; | <block>  // 函数声明或者定义
<defdata> -> ID <vardef> | MUL ID <init> //确定变量
<para> -> <type> <paradata> <paralist> | ^ // 确定参数类型
<paradata> -> MUL ID | ID <paradatatail> // 确定参数名称
<paralist> -> ,<type> <paradata> <paralist> | ^  //确定多个参数
<paradatatail> -> [NUM] | ^ // 确定是数组还是变量，返回值为变量
<block> -> { <subprogram> }
<subprogram> -> <localdef> <subprogram> | <statement> <subprogram> | ^
<localdef> -> <type> <defdata> <deflist> //局部参数定义或声明
<expr> -> <assexpr>
<assexpr> -> <orexpr> <asstail>
<asstail> -> = <orexpr> <asstail> | ^
<orexpr> -> <andexpr> <ortail>
<ortail> -> OR <andexpr> <ortail> | ^
<andexpr> -> <cmpexpr> <andtail>
<andtail> -> AND <cmpexpr> <andtai> | ^
<cmpexpr> -> <aloexpr> <cmptail> 
<cmptail> -> <cmps> <aloexpr> <cmptail> | ^
<cmps> -> GT | GE | LT | LE | EQU | NEQU
<aloexpr> -> <item> <alotail>
<alotail> -> <adds> <item> <alotail> | ^
<adds> -> ADD | SUB
<item> -> <factor> <itemtail> 
<itemtail> -> <muls> <factors> <itemtail> | ^
<muls> -> MUL | DIV | MOD
<factor> -> <lop> <factor> | <val>
<lop> -> NOT | SUB | LEA | MUL | INCR | DECR
<val> -> <elem> <rop>
<rop> -> INC | DEC
<elem> -> ID <idexpr> | ( <expr> ) | <literal> 
<idexpr> -> [ <expr> ] | ( <realarg> ) | ^
<realarg> -> <arg> <arglist> | ^
<arglist> -> , <arg> <arglist> | ^
<arg> -> <expr>
<literal> -> NUM | CH | STR
<altexpr> -> <expr> | ^
<statement> -> <altexpr> ; | <whilestat> | <forstat> | <dowhilestat> | <ifstat> | <switchstat> | BREAK | CONTINUE |RETURN ; | RETURN <altexpr> ;
<whilestat> -> WHILE ( <altexpr> )  <block>
<dowhilestat> -> DO <block> WHILE ( <altexpr> ) ; 
<forstat> -> FOR ( <forinit> <altexpr> ; <altexpr> ) <block>
<forinit> -> <localdef> | <altexpr> ;
<ifstat> -> IF ( <expr> ) <block> <elsestat>
<elsestat> -> ELSE <block> | ^
<switchstat> -> SWITCH ( <expr> ) { <casestat> }
<casestat> -> CASE <caselabel> :  <subprogram> <casestat> | DEFAULT : <subprogram>
<caselabel> -> <literal>
```