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

FIRST SET
```
FIRST(<program>) = extern | TYPE_FIRST  | ^
FIRST(<segment>) = extern | TYPE_FIRST 
FIRST(<type>) = INT | CHAR | VOID 
FIRST(<def>) = ID | MUL
FIRST(<idtail>) = [ | EXPR_FIRST | , | ; | ( 
FIRST(<init>) = = EXPR_FIRST | ^
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
FOLLOW(<localdef>) = FIRST(<subprogram>)
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
FOLLOW(<localdef>) = TYPE_FIRST  | STATEMENT_FIRST
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