%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estructuras.h"
#include "symtab.h"
#include "semantic.h"

extern FILE *yyin;
int yylex(void);
void yyerror(const char *s);
extern int yylineno; 

Node *ast_root = NULL;

const char* dtype_to_str(DataType t) {
    if(t == TYPE_INT) return "int";
    if(t == TYPE_FLOAT) return "float";
    if(t == TYPE_BOOLEAN) return "boolean";
    if(t == TYPE_VOID) return "void";
    return "unknown";
}

%}

%union {
    char *text;
    struct Node *node;
    DataType data_type;
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

/* Rule declarations */
%type <node> program
%type <node> declarations
%type <node> var_decl
%type <node> block
%type <node> statement
%type <node> statement_list
%type <node> expr
%type <node> literal
%type <node> method_call
%type <node> var_decl_list
%type <node> id_list_tail
%type <node> method_decl_list
%type <node> method_decl
%type <node> param_list
%type <node> param_list_opt
%type <node> param
%type <node> args_opt
%type <node> arg_list
%type <node> expr_opt

%type <data_type> type

%%

/* Grammar rules */

program:
    declarations {
        $$ = $1;
        ast_root = $$;
    }
    ;

declarations:
    var_decl declarations {
        $$ = $2;
        addChild($$, $1);
    }
    | method_decl method_decl_list {
        $$ = createNode(PROGRAM_NODE, NULL);
        addChild($$, $1);
        for(int i=0; i<$2->child_count; i++) {
            addChild($$, $2->children[i]);
        }
        free($2->children); free($2);
    }
    | { $$ = createNode(PROGRAM_NODE, NULL); }
    ;

/* Variables */

var_decl_list:
    var_decl_list var_decl {
        $$ = $1;
        addChild($$, $2);
    }
    | { $$ = createNode(BLOCK_NODE, "temp_var_decls"); }
    ;

var_decl:
      type ID id_list_tail ';' {
          $$ = createNode(VAR_DECL_NODE, (char*)dtype_to_str($1));
          addChild($$, createNode(ID_NODE, $2));
          for(int i=0; i<$3->child_count; i++) {
              addChild($$, $3->children[i]);
          }
          free($3->children); free($3);
      }
    ;

id_list_tail:
    id_list_tail ',' ID {
        $$ = $1;
        addChild($$, createNode(ID_NODE, $3));
    }
    | { $$ = createNode(ID_NODE, "temp_ids"); }
    ;

/* Methods */

method_decl_list:
    method_decl_list method_decl {
        $$ = $1;
        addChild($$, $2);
    }
    | { $$ = createNode(PROGRAM_NODE, "temp_methods"); }
    ;

method_decl:
      type ID '(' param_list_opt ')' block {
          $$ = createNode(METHOD_NODE, (char*)dtype_to_str($1));
          addChild($$, createNode(ID_NODE, $2));
          addChild($$, $4);
          addChild($$, $6);
      }
    | VOID ID '(' param_list_opt ')' block {
          $$ = createNode(METHOD_NODE, "void");
          addChild($$, createNode(ID_NODE, $2));
          addChild($$, $4);
          addChild($$, $6);
      }
    ;


param_list:
      param {
          $$ = createNode(PARAMETER_NODE, "list");
          addChild($$, $1);
      }
    | param_list ',' param {
          $$ = $1;
          addChild($$, $3);
      }
    ;

param_list_opt:
    param_list { $$ = $1; }
    | { $$ = createNode(PARAMETER_NODE, "list_empty"); }
    ;

param:
      type ID {
          $$ = createNode(PARAMETER_NODE, (char*)dtype_to_str($1));
          addChild($$, createNode(ID_NODE, $2));
      }
    ;

/* Types */

type:
      INT { $$ = TYPE_INT; }
    | BOOLEAN { $$ = TYPE_BOOLEAN; }
    | FLOAT { $$ = TYPE_FLOAT; }
    ;

/* Blocks */

block:
      '{' var_decl_list statement_list '}' {
          $$ = createNode(BLOCK_NODE, NULL);
          // Add all var decls
          for(int i=0; i<$2->child_count; i++) addChild($$, $2->children[i]);
          free($2->children); free($2);
          // Add all statements
          for(int i=0; i<$3->child_count; i++) addChild($$, $3->children[i]);
          free($3->children); free($3);
      }
    ;

/* Statements */

statement_list:
    statement_list statement {
        $$ = $1;
        if ($2 != NULL) addChild($$, $2); // Ignore empty statements
    }
    | { $$ = createNode(BLOCK_NODE, "temp_stmts"); }
    ;

statement:
      ID '=' expr ';' {
          $$ = createNode(ASSIGNMENT_NODE, NULL);
          addChild($$, createNode(ID_NODE, $1));
          addChild($$, $3);
      }
    | method_call ';' {
          $$ = $1;
      }
    | IF '(' expr ')' block {
          $$ = createNode(IF_NODE, NULL);
          addChild($$, $3);
          addChild($$, $5);
      }
    | IF '(' expr ')' block ELSE block {
          $$ = createNode(IF_NODE, "else");
          addChild($$, $3);
          addChild($$, $5);
          addChild($$, $7);
      }
    | WHILE '(' expr ')' block {
          $$ = createNode(WHILE_NODE, NULL);
          addChild($$, $3);
          addChild($$, $5);
      }
    | RETURN expr_opt ';' {
          $$ = createNode(RETURN_NODE, NULL);
          if ($2 != NULL) addChild($$, $2);
      }
    | ';' { $$ = NULL; }
    | block { $$ = $1; }
    ;

expr_opt:
    expr { $$ = $1; }
    | { $$ = NULL; }
    ;

method_call:
      ID '(' args_opt ')' {
          $$ = createNode(CALL_NODE, $1);
          if ($3 != NULL) {
              for(int i=0; i<$3->child_count; i++) addChild($$, $3->children[i]);
              free($3->children); free($3);
          }
      }
    ;

args_opt:
    arg_list { $$ = $1; }
    | { $$ = NULL; }
    ;

arg_list:
      expr {
          $$ = createNode(CALL_NODE, "args");
          addChild($$, $1);
      }
    | arg_list ',' expr {
          $$ = $1;
          addChild($$, $3);
      }
    ;

/* Expressions */

expr:
      ID { $$ = createNode(ID_NODE, $1); }
    | method_call { $$ = $1; }
    | literal { $$ = $1; }
    | expr '+' expr { $$ = createNode(OPERATION_NODE, "+"); addChild($$, $1); addChild($$, $3); }
    | expr '-' expr { $$ = createNode(OPERATION_NODE, "-"); addChild($$, $1); addChild($$, $3); }
    | expr '*' expr { $$ = createNode(OPERATION_NODE, "*"); addChild($$, $1); addChild($$, $3); }
    | expr '/' expr { $$ = createNode(OPERATION_NODE, "/"); addChild($$, $1); addChild($$, $3); }
    | expr '%' expr { $$ = createNode(OPERATION_NODE, "%"); addChild($$, $1); addChild($$, $3); }
    | expr '<' expr { $$ = createNode(OPERATION_NODE, "<"); addChild($$, $1); addChild($$, $3); }
    | expr '>' expr { $$ = createNode(OPERATION_NODE, ">"); addChild($$, $1); addChild($$, $3); }
    | expr EQUALS expr { $$ = createNode(OPERATION_NODE, "=="); addChild($$, $1); addChild($$, $3); }
    | expr AND expr { $$ = createNode(OPERATION_NODE, "&&"); addChild($$, $1); addChild($$, $3); }
    | expr OR expr { $$ = createNode(OPERATION_NODE, "||"); addChild($$, $1); addChild($$, $3); }
    | '-' expr %prec UMINUS { $$ = createNode(OPERATION_NODE, "- (unary)"); addChild($$, $2); }
    | NOT expr { $$ = createNode(OPERATION_NODE, "!"); addChild($$, $2); }
    | '(' expr ')' { $$ = $2; }
    ;

literal:
      INT_LITERAL { $$ = createNode(LITERAL_NODE, $1); }
    | FLOAT_LITERAL { $$ = createNode(LITERAL_NODE, $1); }
    | TRUE { $$ = createNode(LITERAL_NODE, "true"); }
    | FALSE { $$ = createNode(LITERAL_NODE, "false"); }
    ;

%%

void yyerror(const char *s) {
  fprintf(stderr, "Error en la línea %d: %s\n", yylineno, s); 
}

void main(int argc, char** argv) {
  symtab_init();
  ++argv, --argc;
  if (argc > 0)
    yyin = fopen(argv[0], "r");
  else
    yyin = stdin;

  if (yyparse() == 0) {
      printf("Succesful.\n\n");
      printf("=== AST ===\n");
      printAST(ast_root);
      printf("\n=== SEMANTIC ANALYSIS & SYMTAB ===\n");
      analyze_semantics(ast_root);
      freeAST(ast_root);
  }
}

int yywrap(void) {
  return 1;
}
