%{
#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin;
int yylex(void);
void yyerror(const char *s);
extern int yylineno; 

%}

%union {
    char *text;
}

%define parse.error verbose

/* Tokens definition */
%token INT BOOLEAN FLOAT VOID
%token IF ELSE WHILE RETURN
%token TRUE FALSE
%token AND OR NOT EQUALS
%token <text> ID INT_LITERAL FLOAT_LITERAL

/* Operator Precedence and Associativity */
%left OR
%left AND
%left EQUALS '<' '>'
%left '+' '-'
%left '*' '/' '%'
%right NOT UMINUS

%%

/* Grammar rules */

program:
      declarations
    ;

declarations:
      /* empty */
    | declarations declaration
    ;

declaration:
      type ID decl_tail
    | VOID ID method_tail
    ;

decl_tail:
      ',' id_list ';'
    | ';'
    | method_tail
    ;

method_tail:
      '(' params_opt ')' block
    ;

id_list:
      ID
    | id_list ',' ID
    ;

var_decl_list:
      /* empty */
    | var_decl_list var_decl
    ;

var_decl:
      type ID id_list_tail ';'
    ;

id_list_tail:
      /* empty */
    | id_list_tail ',' ID
    ;

params_opt:
      /* empty */
    | param_list
    ;

param_list:
      param
    | param_list ',' param
    ;

param:
      type ID
    ;

block:
      '{' var_decl_list statement_list '}'
    ;

statement_list:
      /* empty */
    | statement_list statement
    ;

type:
      INT
    | BOOLEAN
    | FLOAT
    ;

statement:
      ID '=' expr ';'
    | method_call ';'
    | IF '(' expr ')' block
    | IF '(' expr ')' block ELSE block
    | WHILE '(' expr ')' block
    | RETURN expr_opt ';'
    | ';'
    | block
    ;

expr_opt:
      /* empty */
    | expr
    ;

method_call:
      ID '(' args_opt ')'
    ;

args_opt:
      /* empty */
    | arg_list
    ;

arg_list:
      expr
    | arg_list ',' expr
    ;

expr:
      ID
    | method_call
    | literal
    | expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | expr '%' expr
    | expr '<' expr
    | expr '>' expr
    | expr EQUALS expr
    | expr AND expr
    | expr OR expr
    | '-' expr %prec UMINUS
    | NOT expr
    | '(' expr ')'
    ;

literal:
      INT_LITERAL
    | FLOAT_LITERAL
    | TRUE
    | FALSE
    ;

%%

void yyerror(const char *s) {
  fprintf(stderr, "Error en la línea %d: %s\n", yylineno, s); 
}

void main(int argc, char** argv) {
  ++argv, --argc;
  if (argc > 0)
    yyin = fopen(argv[0], "r");
  else
    yyin = stdin;

  if (yyparse() == 0) {
      printf("Parseo exitoso.\n");
  }
}

int yywrap(void) {
  return 1;
}
