%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estructuras.h"
#include "symtab.h"
#include "semantic.h"
#include "tac.h"
#include "codegen.h"
#include "asm.h"

extern FILE *yyin;
int yylex(void);
void yyerror(const char *s);
extern int yylineno;
extern int lexic_error_count; 

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
%token EXTERN
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
%type <node> extern_decl_list
%type <node> extern_decl
%type <node> global_declarations
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
    extern_decl_list global_declarations {
        $$ = createNode(PROGRAM_NODE, NULL);
        Node *decls = createNode(DECLARATIONS_NODE, NULL);
        for (int i = 0; i < $1->child_count; i++) {
            addChild(decls, $1->children[i]);
        }
        for (int i = 0; i < $2->child_count; i++) {
            addChild(decls, $2->children[i]);
        }
        free($1->children); free($1);
        free($2->children); free($2);
        addChild($$, decls);
        ast_root = $$;
    }
    ;

extern_decl_list:
    extern_decl_list extern_decl {
        $$ = $1;
        addChild($$, $2);
    }
    | { $$ = createNode(DECLARATIONS_NODE, NULL); }
    ;

extern_decl:
      EXTERN type ID '(' param_list_opt ')' ';' {
          $$ = createNode(EXTERN_METHOD_NODE, (char*)dtype_to_str($2));
          addChild($$, createNode(ID_NODE, $3));
          addChild($$, $5);
      }
    | EXTERN VOID ID '(' param_list_opt ')' ';' {
          $$ = createNode(EXTERN_METHOD_NODE, "void");
          addChild($$, createNode(ID_NODE, $3));
          addChild($$, $5);
      }
    ;

global_declarations:
    var_decl global_declarations {
        $$ = $2;
        prependChild($$, $1);
    }
    | method_decl method_decl_list {
        $$ = createNode(DECLARATIONS_NODE, NULL);
        addChild($$, $1);
        for(int i=0; i<$2->child_count; i++) {
            addChild($$, $2->children[i]);
        }
        free($2->children); free($2);
    }
    | { $$ = createNode(DECLARATIONS_NODE, NULL); }
    ;

/* Variables */


var_decl_list:
    var_decl_list var_decl {
        $$ = $1;
        addChild($$, $2);
    }
    | { $$ = createNode(VAR_DECL_LIST_NODE, NULL); }
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
    | { $$ = createNode(ID_LIST_NODE, NULL); }
    ;

/* Methods */


method_decl_list:
    method_decl_list method_decl {
        $$ = $1;
        addChild($$, $2);
    }
    | { $$ = createNode(METHOD_DECL_LIST_NODE, NULL); }
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
          $$ = createNode(PARAM_LIST_NODE, NULL);
          addChild($$, $1);
      }
    | param_list ',' param {
          $$ = $1;
          addChild($$, $3);
      }
    ;

param_list_opt:
    param_list { $$ = $1; }
    | { $$ = createNode(PARAM_LIST_NODE, "empty"); }
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
          addChild($$, $2);
          addChild($$, $3);
      }
    ;

/* Statements */

statement_list:
    statement_list statement {
        $$ = $1;
        if ($2 != NULL) addChild($$, $2);
    }
    | { $$ = createNode(STATEMENT_LIST_NODE, NULL); }
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
              addChild($$, $3);
          }
      }
    ;

args_opt:
    arg_list { $$ = $1; }
    | { $$ = createNode(ARG_LIST_NODE, "empty"); }
    ;

arg_list:
      expr {
          $$ = createNode(ARG_LIST_NODE, NULL);
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
  fprintf(stderr, "Error on line: %d: %s\n", yylineno, s); 
}

int main(int argc, char** argv) {
  symtab_init();
  char* input_filename = NULL;
  bool require_main = true;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--module") == 0) {
      require_main = false;
    } else if (argv[i][0] != '-' && input_filename == NULL) {
      input_filename = argv[i];
    }
  }

  if (input_filename != NULL) {
    yyin = fopen(input_filename, "r");
    if (!yyin) {
      fprintf(stderr, "Error: could not open file %s\n", input_filename);
      return 1;
    }
  } else {
    yyin = stdin;
  }

  if (yyparse() == 0) {
      printf("successful parsing.\n\n");
      printf("=== AST ===\n");
      printAST(ast_root);
      printf("\n=== SEMANTIC ANALYSIS & SYMTAB ===\n");
      int sem_errors = analyze_semantics(ast_root, require_main);
      if (sem_errors == 0 && lexic_error_count == 0) {
          printf("\n=== INTERMEDIATE CODE (TAC) ===\n");
          TacList *tac = codegen_generate(ast_root);
          tac_print(stdout, tac);

          if (input_filename != NULL) {
              char ci_filename[512];
              strncpy(ci_filename, input_filename, sizeof(ci_filename) - 1);
              ci_filename[sizeof(ci_filename) - 1] = '\0';
              char *dot = strrchr(ci_filename, '.');
              if (dot) {
                  strcpy(dot, ".ci");
              } else {
                  strncat(ci_filename, ".ci", sizeof(ci_filename) - strlen(ci_filename) - 1);
              }
              FILE *f_ci = fopen(ci_filename, "w");
              if (f_ci) {
                  tac_print(f_ci, tac);
                  fclose(f_ci);
                  printf("\n[Archivo .ci generado: %s]\n", ci_filename);
              }

              char s_filename[512];
              strncpy(s_filename, input_filename, sizeof(s_filename) - 1);
              s_filename[sizeof(s_filename) - 1] = '\0';
              char *dot_s = strrchr(s_filename, '.');
              if (dot_s) {
                  strcpy(dot_s, ".s");
              } else {
                  strncat(s_filename, ".s", sizeof(s_filename) - strlen(s_filename) - 1);
              }
              FILE *f_s = fopen(s_filename, "w");
              if (f_s) {
                  asm_generate(f_s, tac);
                  fclose(f_s);
                  printf("[Archivo .s generado: %s]\n", s_filename);
              }

              if (require_main) {
                  char out_filename[512];
                  strncpy(out_filename, input_filename, sizeof(out_filename) - 1);
                  out_filename[sizeof(out_filename) - 1] = '\0';
                  char *dot_out = strrchr(out_filename, '.');
                  if (dot_out) {
                      strcpy(dot_out, ".out");
                  } else {
                      strncat(out_filename, ".out", sizeof(out_filename) - strlen(out_filename) - 1);
                  }
                  char gcc_cmd[2048];
                  snprintf(gcc_cmd, sizeof(gcc_cmd), "gcc -no-pie %s -o %s 2>/dev/null", s_filename, out_filename);
                  int ret = system(gcc_cmd);
                  if (ret == 0) {
                      printf("[Archivo binario generado: %s]\n", out_filename);
                  }
              }
          }
          tac_free(tac);
      }
      freeAST(ast_root);
  }
  return 0;
}

int yywrap(void) {
  return 1;
}
