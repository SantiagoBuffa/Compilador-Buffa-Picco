%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "estructuras.h"
#include "symtab.h"
#include "semantic.h"
#include "tac.h"
#include "codegen.h"
#include "asm.h"
#include "ast_optimizer.h"

extern FILE *yyin;
int yylex(void);
void yyerror(const char *s);
extern int yylineno;
extern int lexic_error_count; 

Node *ast_root = NULL;
bool debug_mode = false;

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

typedef enum {
    TARGET_FULL,
    TARGET_SCAN,
    TARGET_PARSE,
    TARGET_CODINTER,
    TARGET_ASSEMBLY
} CompilationTarget;

static void print_usage(const char* prog) {
    printf("Usage: %s [options] <source_file>\n\n", prog);
    printf("Options:\n");
    printf("  -o <output>        Specify output file name\n");
    printf("  -target <stage>    Compilation target stage:\n");
    printf("                       scan: Stop after lexical analysis\n");
    printf("                       parse: Stop after syntactic and semantic analysis\n");
    printf("                       codinter: Stop after intermediate code (TAC) generation\n");
    printf("                       assembly: Stop after assembly code generation\n");
    printf("  -opt [opt ...]     Enable optimizations (space-separated list):\n");
    printf("                       fold (or const): Constant folding and propagation\n");
    printf("                       sc (or shortcircuit): Short-circuit boolean evaluation\n");
    printf("                       dce (or deadcode): Dead code elimination\n");
    printf("                       alg (or algebraic): Algebraic simplifications and strength reduction\n");
    printf("                       all: Enable all optimizations (default when -opt has no args)\n");
    printf("  -debug             Print debug information to stdout\n");
    printf("  -c, --module       Compile as module (do not require main method)\n");
    printf("  -h, --help         Display this help message\n");
}

static bool has_ctds_extension(const char* filename) {
    if (!filename) return false;
    const char* dot = strrchr(filename, '.');
    return (dot && strcmp(dot, ".ctds") == 0);
}

static bool file_exists(const char* path) {
    return (access(path, F_OK) == 0);
}

static void derive_filename(const char* input, const char* ext, char* out, size_t size) {
    strncpy(out, input, size - 1);
    out[size - 1] = '\0';
    char* dot = strrchr(out, '.');
    if (dot) {
        *dot = '\0';
    }
    strncat(out, ext, size - strlen(out) - 1);
}

int main(int argc, char** argv) {
    symtab_init();

    char* input_filename = NULL;
    char* output_filename = NULL;
    CompilationTarget target = TARGET_FULL;
    bool require_main = true;
    bool opt_specified = false;

    OptConfig opt_config = {
        .constant_folding = false,
        .short_circuit = false,
        .dead_code_elim = false,
        .algebraic = false
    };

    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 >= argc || argv[i + 1][0] == '-') {
                fprintf(stderr, "Error: Option -o requires an argument\n");
                print_usage(argv[0]);
                return 1;
            }
            output_filename = argv[++i];
        } else if (strcmp(argv[i], "-target") == 0) {
            if (i + 1 >= argc || argv[i + 1][0] == '-') {
                fprintf(stderr, "Error: Option -target requires an argument\n");
                print_usage(argv[0]);
                return 1;
            }
            i++;
            if (strcmp(argv[i], "scan") == 0) {
                target = TARGET_SCAN;
            } else if (strcmp(argv[i], "parse") == 0) {
                target = TARGET_PARSE;
            } else if (strcmp(argv[i], "codinter") == 0) {
                target = TARGET_CODINTER;
            } else if (strcmp(argv[i], "assembly") == 0) {
                target = TARGET_ASSEMBLY;
            } else {
                fprintf(stderr, "Error: Unknown target '%s'. Valid targets: scan, parse, codinter, assembly\n", argv[i]);
                return 1;
            }
        } else if (strcmp(argv[i], "-opt") == 0) {
            opt_specified = true;
            while (i + 1 < argc && argv[i + 1][0] != '-') {
                const char* opt = argv[i + 1];
                if (strcmp(opt, "fold") == 0 || strcmp(opt, "const") == 0) {
                    opt_config.constant_folding = true;
                    i++;
                } else if (strcmp(opt, "sc") == 0 || strcmp(opt, "shortcircuit") == 0) {
                    opt_config.short_circuit = true;
                    i++;
                } else if (strcmp(opt, "dce") == 0 || strcmp(opt, "deadcode") == 0) {
                    opt_config.dead_code_elim = true;
                    i++;
                } else if (strcmp(opt, "alg") == 0 || strcmp(opt, "algebraic") == 0) {
                    opt_config.algebraic = true;
                    i++;
                } else if (strcmp(opt, "all") == 0) {
                    opt_config.constant_folding = true;
                    opt_config.short_circuit = true;
                    opt_config.dead_code_elim = true;
                    opt_config.algebraic = true;
                    i++;
                } else {
                    if (has_ctds_extension(opt) || (input_filename == NULL && file_exists(opt))) {
                        break;
                    } else {
                        fprintf(stderr, "Error: Unknown optimization '%s'. Valid options: fold, sc, dce, alg, all\n", opt);
                        return 1;
                    }
                }
            }
        } else if (strcmp(argv[i], "-debug") == 0) {
            debug_mode = true;
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--module") == 0) {
            require_main = false;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Error: Unknown option '%s'\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        } else {
            if (input_filename == NULL) {
                input_filename = argv[i];
            } else {
                fprintf(stderr, "Error: Multiple input files specified ('%s' and '%s')\n", input_filename, argv[i]);
                return 1;
            }
        }
        i++;
    }

    if (opt_specified && !opt_config.constant_folding && !opt_config.short_circuit &&
        !opt_config.dead_code_elim && !opt_config.algebraic) {
        opt_config.constant_folding = true;
        opt_config.short_circuit = true;
        opt_config.dead_code_elim = true;
        opt_config.algebraic = true;
    }

    if (input_filename == NULL) {
        fprintf(stderr, "Error: No input file specified\n");
        print_usage(argv[0]);
        return 1;
    }

    yyin = fopen(input_filename, "r");
    if (!yyin) {
        fprintf(stderr, "Error: could not open file %s\n", input_filename);
        return 1;
    }

    // TARGET: scan
    if (target == TARGET_SCAN) {
        int tok;
        while ((tok = yylex()) != 0) {
            if (tok == ID || tok == INT_LITERAL || tok == FLOAT_LITERAL) {
                free(yylval.text);
            }
        }
        fclose(yyin);
        if (lexic_error_count > 0) {
            return 1;
        }
        if (debug_mode) {
            printf("successful scanning.\n");
        }
        return 0;
    }

    // Parsing
    if (yyparse() != 0 || lexic_error_count > 0) {
        if (ast_root) freeAST(ast_root);
        fclose(yyin);
        return 1;
    }
    fclose(yyin);

    if (debug_mode) {
        printf("successful parsing.\n\n");
        printf("=== AST (ORIGINAL) ===\n");
        printAST(ast_root);
        printf("\n=== SEMANTIC ANALYSIS & SYMTAB ===\n");
    }

    int sem_errors = analyze_semantics(ast_root, require_main);
    if (sem_errors > 0) {
        freeAST(ast_root);
        return 1;
    }

    // TARGET: parse
    if (target == TARGET_PARSE) {
        freeAST(ast_root);
        return 0;
    }

    // AST Optimizations
    if (opt_config.constant_folding || opt_config.short_circuit ||
        opt_config.dead_code_elim || opt_config.algebraic) {
        ast_root = optimize_ast_with_config(ast_root, opt_config);
        if (debug_mode) {
            printf("\n=== AST (OPTIMIZED) ===\n");
            printAST(ast_root);
        }
    }

    // TAC Generation
    if (debug_mode) {
        printf("\n=== INTERMEDIATE CODE (TAC) ===\n");
    }
    TacList* tac = codegen_generate(ast_root);
    if (debug_mode) {
        tac_print(stdout, tac);
    }

    char default_ci[512];
    char default_s[512];
    char default_out[512];
    derive_filename(input_filename, ".ci", default_ci, sizeof(default_ci));
    derive_filename(input_filename, ".s", default_s, sizeof(default_s));
    derive_filename(input_filename, ".out", default_out, sizeof(default_out));

    // TARGET: codinter
    if (target == TARGET_CODINTER) {
        const char* ci_dest = (output_filename != NULL) ? output_filename : default_ci;
        FILE* f_ci = fopen(ci_dest, "w");
        if (!f_ci) {
            fprintf(stderr, "Error: could not open output file %s for writing\n", ci_dest);
            tac_free(tac);
            freeAST(ast_root);
            return 1;
        }
        tac_print(f_ci, tac);
        fclose(f_ci);
        if (debug_mode) {
            printf("\n[Archivo .ci generado: %s]\n", ci_dest);
        }
        tac_free(tac);
        freeAST(ast_root);
        return 0;
    }

    // Generate intermediate code file (.ci)
    FILE* f_ci = fopen(default_ci, "w");
    if (f_ci) {
        tac_print(f_ci, tac);
        fclose(f_ci);
        if (debug_mode) {
            printf("\n[Archivo .ci generado: %s]\n", default_ci);
        }
    }

    // TARGET: assembly
    if (target == TARGET_ASSEMBLY) {
        const char* s_dest = (output_filename != NULL) ? output_filename : default_s;
        FILE* f_s = fopen(s_dest, "w");
        if (!f_s) {
            fprintf(stderr, "Error: could not open output file %s for writing\n", s_dest);
            tac_free(tac);
            freeAST(ast_root);
            return 1;
        }
        asm_generate(f_s, tac);
        fclose(f_s);
        if (debug_mode) {
            printf("[Archivo .s generado: %s]\n", s_dest);
        }
        tac_free(tac);
        freeAST(ast_root);
        return 0;
    }

    // Generate assembly file (.s)
    FILE* f_s = fopen(default_s, "w");
    if (!f_s) {
        fprintf(stderr, "Error: could not open output file %s for writing\n", default_s);
        tac_free(tac);
        freeAST(ast_root);
        return 1;
    }
    asm_generate(f_s, tac);
    fclose(f_s);
    if (debug_mode) {
        printf("[Archivo .s generado: %s]\n", default_s);
    }

    // TARGET: default (full compilation)
    if (require_main) {
        const char* bin_dest = (output_filename != NULL) ? output_filename : default_out;
        char gcc_cmd[2048];
        snprintf(gcc_cmd, sizeof(gcc_cmd), "gcc -no-pie %s -o %s 2>/dev/null", default_s, bin_dest);
        int ret = system(gcc_cmd);
        if (ret == 0) {
            if (debug_mode) {
                printf("[Archivo binario generado: %s]\n", bin_dest);
            }
        } else {
            fprintf(stderr, "Error: gcc assembly/linking failed for %s\n", default_s);
            tac_free(tac);
            freeAST(ast_root);
            return 1;
        }
    } else if (output_filename != NULL) {
        // Module with custom output: if output ends with .o, assemble with gcc -c
        char gcc_cmd[2048];
        snprintf(gcc_cmd, sizeof(gcc_cmd), "gcc -c %s -o %s 2>/dev/null", default_s, output_filename);
        system(gcc_cmd);
    }

    tac_free(tac);
    freeAST(ast_root);
    return 0;
}

int yywrap(void) {
  return 1;
}
