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
/* Less to more precedence */
%left OR
%left AND
%left EQUALS '<' '>'
%left '+' '-'
%left '*' '/' '%'
%right NOT UMINUS /* auxiliar, allows us to make a distinction for the 
treatment of minus when used in a single number */

%%

/* Grammar rules */

program:
    declarations
    ;

declarations:
    var_decl declarations
    | method_decl method_decl_list
    |
    ;

/* Variables */

var_decl_list:
    var_decl_list var_decl
    | 
    ;

var_decl:
      type ID id_list_tail ';'
    ;

id_list_tail:
    id_list_tail ',' ID
    | 
    ;

/* Methods */

method_decl_list:
    method_decl_list method_decl
    |
    ;

method_decl:
      type ID '(' param_list_opt ')' block
    | VOID ID '(' param_list_opt ')' block
    ;


param_list:
      param
    | param_list ',' param
    ;

param_list_opt:
    param_list
    |
    ; /* Allows for no parameters in declaration */

param:
      type ID
    ;

/* Types */

type:
      INT
    | BOOLEAN
    | FLOAT
    ;

/* Blocks */

block:
      '{' var_decl_list statement_list '}'
    ;

/* Statements */

statement_list:
    statement_list statement
    | 
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
    expr
    | 
    ;

method_call:
      ID '(' args_opt ')'
    ;

args_opt:
    arg_list
    | 
    ;

arg_list:
      expr
    | arg_list ',' expr
    ;

/* Expressions */

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
    | '-' expr %prec UMINUS /* %prec allows to override the normal precedence of '-' for this one rule */
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
