 /* PL词法分析器 */
 /* 功能：能够识别出PL支持的所有单词符号并给出种别值 */
 /* 说明：在下面的begin和end之间添加代码，已经实现了标识符和整常量的识别，你需要完成剩下的部分，加油吧！ */
 /* 提示：因为是顺序匹配，即从上至下依次匹配规则，所以需要合理安排顺序～ */
%{
#include <stdio.h>
%}
 /* begin */
OFSYM           of
ARRAYSYM        array
PROGRAMSYM      program
MODSYM          mod
ANDSYM          and
ORSYM           or
NOTSYM          not
BEGINSYM        begin
ENDSYM          end
IFSYM           if
THENSYM         then
ELSESYM         else
WHILESYM        while
DOSYM           do
CALLSYM         call
CONSTSYM        const
TYPESYM         type
VARSYM          var
PROCSYM         procedure
IDENT			[A-Za-z][A-Za-z0-9]*
INTCON			[\-]?[1-9][0-9]*|0
BECOME          \:\=
COLON           \:
LEQ             \<\=
NEQ             \<\>
LSS             \<
GEQ             \>\=
GTR             \>
PERIOD          \.
CHARCON         \'([^'\\]|\\.)*\'
PLUS            \+
MINUS           \-
TIMES           \*
DIVSYM          \/
LPAREN          \(
RPAREN          \)
EQL             \=
LBRACK          \[
RBRACK          \]
SEMICOLON       \;
COMMA           \,
ERROR           [\~\!\@\#\$\%\^\&\_\\]
 
 /* end */

%%
 /* begin */
{OFSYM}          {printf("of: OFSYM\n");}
{ARRAYSYM}       {printf("array: ARRAYSYM\n");}
{PROGRAMSYM}     {printf("program: PROGRAMSYM\n");}
{MODSYM}         {printf("mod: MODSYM\n");}
{ANDSYM}         {printf("and: ANDSYM\n");}
{ORSYM}          {printf("or: ORSYM\n");}
{NOTSYM}         {printf("not: NOTSYM\n");}
{BEGINSYM}       {printf("begin: BEGINSYM\n");}
{ENDSYM}         {printf("end: ENDSYM\n");}
{IFSYM}          {printf("if: IFSYM\n");}
{THENSYM}        {printf("then: THENSYM\n");}
{ELSESYM}        {printf("else: ELSESYM\n");}
{WHILESYM}       {printf("while: WHILESYM\n");}
{DOSYM}          {printf("do: DOSYM\n");}
{CALLSYM}        {printf("call: CALLSYM\n");}
{CONSTSYM}       {printf("const: CONSTSYM\n");}
{TYPESYM}        {printf("type: TYPESYM\n");}
{VARSYM}         {printf("var: VARSYM\n");}
{PROCSYM}        {printf("procedure: PROCSYM\n");}
{IDENT}          {printf("%s: IDENT\n", yytext);}
{INTCON}         {printf("%s: INTCON\n", yytext);}
{BECOME}         {printf(":=: BECOME\n");}
{COLON}          {printf(":: COLON\n");}
{LEQ}            {printf("<=: LEQ\n");}
{NEQ}            {printf("<>: NEQ\n");}
{LSS}            {printf("<: LSS\n");}
{GEQ}            {printf(">=: GEQ\n");}
{GTR}            {printf(">: GTR\n");}
{PERIOD}         {printf(".: PERIOD\n");}
{CHARCON}        {printf("%s: CHARCON\n", yytext);}
{PLUS}           {printf("+: PLUS\n");}
{MINUS}          {printf("-: MINUS\n");}
{TIMES}          {printf("*: TIMES\n");}
{DIVSYM}         {printf("/: DIVSYM\n");}
{LPAREN}         {printf("(: LPAREN\n");}
{RPAREN}         {printf("): RPAREN\n");}
{EQL}            {printf("=: EQL\n");}
{LBRACK}         {printf("[: LBRACK\n");}
{RBRACK}         {printf("]: RBRACK\n");}
{SEMICOLON}      {printf(";: SEMICOLON\n");}
{COMMA}          {printf(",: COMMA\n");}
{ERROR}          {printf("%s: ERROR\n", yytext);}

 
 /* end */

\n				{}
.				{}
%%
int yywrap() { return 1; }
int main(int argc, char **argv)
{
	if (argc > 1) {
		if (!(yyin = fopen(argv[1], "r"))) {
			perror(argv[1]);
			return 1;
		}
	}
	while (yylex());
	return 0;
}
