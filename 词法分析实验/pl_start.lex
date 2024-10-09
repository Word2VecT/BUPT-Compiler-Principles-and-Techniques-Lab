 /* 简单词法分析器 */
 /* 功能：能够识别出以小写字母ab结尾的所有字符串（仅含大小写字母）并给打印'Hit!' */
 /* 说明：在下面的begin和end之间添加代码，注意格式 */
 /* 提示：你只需要保证合法的输入（以ab结尾的字符串）有结果，不合法的输入将会包含在.规则中～ */
%{
#include <stdio.h>
%}

%%
 /* begin */
[a-zA-Z]*ab    {printf("%s: Hit!", yytext);}
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
