/* 逆波兰符号计算器 */
/* 功能：能够计算出合法的后缀表达式的值，包括加法、减法、乘法、除法、乘方、取反等运算 */
/* 说明：在下面的begin和end之间添加代码，加油吧！ */
/* 提示： */

%{
  #include <ctype.h>
  #include <stdio.h>
  #include <math.h>
  int yylex (void);
  void yyerror (char const *);
%}


%define api.value.type {double}
%token NUM

%% 

/* Grammar rules and actions follow.  */

input:
  %empty
| input line
;

line:
  '\n'
| exp '\n'      { printf ("%.10g\n", $1); }
;

exp:
  NUM           { $$ = $1;           }
  /* begin */
| exp exp '+'   { $$ = $1 + $2;     }
| exp exp '-'   { $$ = $1 - $2;     }
| exp exp '*'   { $$ = $1 * $2;     }
| exp exp '/'   { if ($2 == 0) { yyerror("Division by zero"); $$ = 0; } else $$ = $1 / $2; }
| exp exp '^'   { $$ = pow($1, $2); }
| exp '~'       { $$ = -$1;         }
  /* end */
;

%%

int yylex (void)
{
  int c;

  /* Skip white space.  */
  while ((c = getchar ()) == ' ' || c == '\t')
    continue;

  /* Process numbers.  */
  if (c == '.' || isdigit (c))
    {
      ungetc (c, stdin);
      scanf ("%lf", &yylval);
      return NUM;
    }

  /* Return end-of-input.  */
  if (c == EOF)
    return 0;
  if (c == '!')
    return 0;
  /* Return a single char.  */
  return c;
}

int main (int argc, char** argv)
{
  return yyparse();
}


/* Called by yyparse on error.  */
void yyerror (char const *s)
{
  fprintf (stderr, "%s\n", s);
}