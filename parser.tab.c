/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estructuras.h"
#include "symtab.h"
#include "semantic.h"
#include "tac.h"
#include "codegen.h"

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


#line 100 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INT = 3,                        /* INT  */
  YYSYMBOL_BOOLEAN = 4,                    /* BOOLEAN  */
  YYSYMBOL_FLOAT = 5,                      /* FLOAT  */
  YYSYMBOL_VOID = 6,                       /* VOID  */
  YYSYMBOL_IF = 7,                         /* IF  */
  YYSYMBOL_ELSE = 8,                       /* ELSE  */
  YYSYMBOL_WHILE = 9,                      /* WHILE  */
  YYSYMBOL_RETURN = 10,                    /* RETURN  */
  YYSYMBOL_TRUE = 11,                      /* TRUE  */
  YYSYMBOL_FALSE = 12,                     /* FALSE  */
  YYSYMBOL_AND = 13,                       /* AND  */
  YYSYMBOL_OR = 14,                        /* OR  */
  YYSYMBOL_NOT = 15,                       /* NOT  */
  YYSYMBOL_EQUALS = 16,                    /* EQUALS  */
  YYSYMBOL_EXTERN = 17,                    /* EXTERN  */
  YYSYMBOL_ID = 18,                        /* ID  */
  YYSYMBOL_INT_LITERAL = 19,               /* INT_LITERAL  */
  YYSYMBOL_FLOAT_LITERAL = 20,             /* FLOAT_LITERAL  */
  YYSYMBOL_21_ = 21,                       /* '<'  */
  YYSYMBOL_22_ = 22,                       /* '>'  */
  YYSYMBOL_23_ = 23,                       /* '+'  */
  YYSYMBOL_24_ = 24,                       /* '-'  */
  YYSYMBOL_25_ = 25,                       /* '*'  */
  YYSYMBOL_26_ = 26,                       /* '/'  */
  YYSYMBOL_27_ = 27,                       /* '%'  */
  YYSYMBOL_UMINUS = 28,                    /* UMINUS  */
  YYSYMBOL_29_ = 29,                       /* '('  */
  YYSYMBOL_30_ = 30,                       /* ')'  */
  YYSYMBOL_31_ = 31,                       /* ';'  */
  YYSYMBOL_32_ = 32,                       /* ','  */
  YYSYMBOL_33_ = 33,                       /* '{'  */
  YYSYMBOL_34_ = 34,                       /* '}'  */
  YYSYMBOL_35_ = 35,                       /* '='  */
  YYSYMBOL_YYACCEPT = 36,                  /* $accept  */
  YYSYMBOL_program = 37,                   /* program  */
  YYSYMBOL_extern_decl_list = 38,          /* extern_decl_list  */
  YYSYMBOL_extern_decl = 39,               /* extern_decl  */
  YYSYMBOL_global_declarations = 40,       /* global_declarations  */
  YYSYMBOL_var_decl_list = 41,             /* var_decl_list  */
  YYSYMBOL_var_decl = 42,                  /* var_decl  */
  YYSYMBOL_id_list_tail = 43,              /* id_list_tail  */
  YYSYMBOL_method_decl_list = 44,          /* method_decl_list  */
  YYSYMBOL_method_decl = 45,               /* method_decl  */
  YYSYMBOL_param_list = 46,                /* param_list  */
  YYSYMBOL_param_list_opt = 47,            /* param_list_opt  */
  YYSYMBOL_param = 48,                     /* param  */
  YYSYMBOL_type = 49,                      /* type  */
  YYSYMBOL_block = 50,                     /* block  */
  YYSYMBOL_statement_list = 51,            /* statement_list  */
  YYSYMBOL_statement = 52,                 /* statement  */
  YYSYMBOL_expr_opt = 53,                  /* expr_opt  */
  YYSYMBOL_method_call = 54,               /* method_call  */
  YYSYMBOL_args_opt = 55,                  /* args_opt  */
  YYSYMBOL_arg_list = 56,                  /* arg_list  */
  YYSYMBOL_expr = 57,                      /* expr  */
  YYSYMBOL_literal = 58                    /* literal  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   180

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  36
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  64
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  124

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   276


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    27,     2,     2,
      29,    30,    25,    23,    32,    24,     2,    26,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    31,
      21,    35,    22,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    33,     2,    34,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    28
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    84,    84,   101,   105,   109,   114,   122,   126,   134,
     141,   145,   149,   161,   165,   172,   176,   180,   186,   196,
     200,   207,   208,   212,   221,   222,   223,   229,   239,   243,
     247,   252,   255,   260,   266,   271,   275,   276,   280,   281,
     285,   294,   295,   299,   303,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   331,   332,   333,   334
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INT", "BOOLEAN",
  "FLOAT", "VOID", "IF", "ELSE", "WHILE", "RETURN", "TRUE", "FALSE", "AND",
  "OR", "NOT", "EQUALS", "EXTERN", "ID", "INT_LITERAL", "FLOAT_LITERAL",
  "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "'%'", "UMINUS", "'('", "')'",
  "';'", "','", "'{'", "'}'", "'='", "$accept", "program",
  "extern_decl_list", "extern_decl", "global_declarations",
  "var_decl_list", "var_decl", "id_list_tail", "method_decl_list",
  "method_decl", "param_list", "param_list_opt", "param", "type", "block",
  "statement_list", "statement", "expr_opt", "method_call", "args_opt",
  "arg_list", "expr", "literal", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-66)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -66,     6,   145,   -66,   -66,   -66,   -66,   -11,   100,   -66,
     -66,   115,   -66,    -7,    19,    23,    45,   -66,   115,    22,
     130,    36,    38,   -66,    54,   130,   -28,    41,    46,   -66,
      57,   130,   130,    22,    51,   -66,    68,   130,   103,   -66,
     133,   137,   103,   -66,   -66,   -66,   -66,   114,   129,   -66,
     130,   -66,   -66,   -66,   128,    35,   -66,   132,   135,    59,
     -21,   -66,   -66,   -66,   -66,   138,    59,    59,   -66,   -66,
      59,   139,   -66,   -66,    59,    59,   140,   -66,   116,   -66,
      59,    59,   -66,    71,    86,   -66,   -66,   101,   -66,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,   142,
     141,   116,    33,   103,   103,   -66,    -4,   131,    12,    12,
      12,    64,    64,   -66,   -66,   -66,   -66,    59,   -66,   162,
     -66,   116,   103,   -66
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     9,     1,    24,    25,    26,     0,     0,     3,
       2,     9,    16,     0,     0,     0,     0,     7,     8,    14,
      22,     0,     0,    15,     0,    22,     0,    21,     0,    19,
       0,    22,    22,     0,     0,    12,     0,     0,     0,    23,
       0,     0,     0,    13,    20,    11,    18,     0,     0,    17,
      29,     6,     5,    10,     0,     0,    14,     0,     0,    39,
       0,    36,    27,    37,    28,     0,     0,     0,    63,    64,
       0,    45,    61,    62,     0,     0,     0,    46,    38,    47,
      42,     0,    31,     0,     0,    59,    58,     0,    35,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      41,    43,     0,     0,     0,    60,    56,    57,    55,    53,
      54,    48,    49,    50,    51,    52,    40,     0,    30,    32,
      34,    44,     0,    33
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -66,   -66,   -66,   -66,   163,   -66,   125,   -66,   -66,   158,
     -66,   134,   143,    32,   -42,   -66,   -66,   -66,   122,   -66,
     -66,   -65,   -66
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,     9,    10,    50,    11,    26,    18,    12,
      27,    28,    29,    30,    46,    55,    64,    76,    77,    99,
     100,    78,    79
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      49,    83,    84,    35,    36,    85,     3,    14,    80,    86,
      87,    19,    91,    63,    81,   101,   102,    92,    93,    94,
      95,    96,    97,    98,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,    13,    94,    95,    96,    97,    98,
      16,    21,    57,    13,    58,    59,    89,    90,    20,    91,
      24,    25,   121,    60,    92,    93,    94,    95,    96,    97,
      98,   119,   120,    22,   118,    31,    61,    32,    45,    62,
      68,    69,    33,    37,    70,    39,    38,    71,    72,    73,
     123,    42,    54,    74,    89,    90,    43,    91,    75,    96,
      97,    98,    92,    93,    94,    95,    96,    97,    98,    89,
      90,   103,    91,     4,     5,     6,    15,    92,    93,    94,
      95,    96,    97,    98,    89,    90,   104,    91,     4,     5,
       6,     7,    92,    93,    94,    95,    96,    97,    98,    89,
      90,   105,    91,     4,     5,     6,    45,    92,    93,    94,
      95,    96,    97,    98,    89,    51,    56,    91,     4,     5,
       6,     7,    92,    93,    94,    95,    96,    97,    98,    34,
      52,    66,     8,    47,    67,    40,    41,    48,    80,    82,
     122,    88,   116,   117,    17,    53,    23,    65,     0,     0,
      44
};

static const yytype_int8 yycheck[] =
{
      42,    66,    67,    31,    32,    70,     0,    18,    29,    74,
      75,    18,    16,    55,    35,    80,    81,    21,    22,    23,
      24,    25,    26,    27,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,     2,    23,    24,    25,    26,    27,
       8,    18,     7,    11,     9,    10,    13,    14,    29,    16,
      18,    29,   117,    18,    21,    22,    23,    24,    25,    26,
      27,   103,   104,    18,    31,    29,    31,    29,    33,    34,
      11,    12,    18,    32,    15,    18,    30,    18,    19,    20,
     122,    30,    50,    24,    13,    14,    18,    16,    29,    25,
      26,    27,    21,    22,    23,    24,    25,    26,    27,    13,
      14,    30,    16,     3,     4,     5,     6,    21,    22,    23,
      24,    25,    26,    27,    13,    14,    30,    16,     3,     4,
       5,     6,    21,    22,    23,    24,    25,    26,    27,    13,
      14,    30,    16,     3,     4,     5,    33,    21,    22,    23,
      24,    25,    26,    27,    13,    31,    18,    16,     3,     4,
       5,     6,    21,    22,    23,    24,    25,    26,    27,    25,
      31,    29,    17,    30,    29,    31,    32,    30,    29,    31,
       8,    31,    30,    32,    11,    50,    18,    55,    -1,    -1,
      37
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    37,    38,     0,     3,     4,     5,     6,    17,    39,
      40,    42,    45,    49,    18,     6,    49,    40,    44,    18,
      29,    18,    18,    45,    49,    29,    43,    46,    47,    48,
      49,    29,    29,    18,    47,    31,    32,    32,    30,    18,
      47,    47,    30,    18,    48,    33,    50,    30,    30,    50,
      41,    31,    31,    42,    49,    51,    18,     7,     9,    10,
      18,    31,    34,    50,    52,    54,    29,    29,    11,    12,
      15,    18,    19,    20,    24,    29,    53,    54,    57,    58,
      29,    35,    31,    57,    57,    57,    57,    57,    31,    13,
      14,    16,    21,    22,    23,    24,    25,    26,    27,    55,
      56,    57,    57,    30,    30,    30,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    30,    32,    31,    50,
      50,    57,     8,    50
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    36,    37,    38,    38,    39,    39,    40,    40,    40,
      41,    41,    42,    43,    43,    44,    44,    45,    45,    46,
      46,    47,    47,    48,    49,    49,    49,    50,    51,    51,
      52,    52,    52,    52,    52,    52,    52,    52,    53,    53,
      54,    55,    55,    56,    56,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    58,    58,    58,    58
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     2,     0,     7,     7,     2,     2,     0,
       2,     0,     4,     3,     0,     2,     0,     6,     6,     1,
       3,     1,     0,     2,     1,     1,     1,     4,     2,     0,
       4,     2,     5,     7,     5,     3,     1,     1,     1,     0,
       4,     1,     0,     1,     3,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       3,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: extern_decl_list global_declarations  */
#line 84 "parser.y"
                                         {
        (yyval.node) = createNode(PROGRAM_NODE, NULL);
        Node *decls = createNode(DECLARATIONS_NODE, NULL);
        for (int i = 0; i < (yyvsp[-1].node)->child_count; i++) {
            addChild(decls, (yyvsp[-1].node)->children[i]);
        }
        for (int i = 0; i < (yyvsp[0].node)->child_count; i++) {
            addChild(decls, (yyvsp[0].node)->children[i]);
        }
        free((yyvsp[-1].node)->children); free((yyvsp[-1].node));
        free((yyvsp[0].node)->children); free((yyvsp[0].node));
        addChild((yyval.node), decls);
        ast_root = (yyval.node);
    }
#line 1518 "parser.tab.c"
    break;

  case 3: /* extern_decl_list: extern_decl_list extern_decl  */
#line 101 "parser.y"
                                 {
        (yyval.node) = (yyvsp[-1].node);
        addChild((yyval.node), (yyvsp[0].node));
    }
#line 1527 "parser.tab.c"
    break;

  case 4: /* extern_decl_list: %empty  */
#line 105 "parser.y"
      { (yyval.node) = createNode(DECLARATIONS_NODE, NULL); }
#line 1533 "parser.tab.c"
    break;

  case 5: /* extern_decl: EXTERN type ID '(' param_list_opt ')' ';'  */
#line 109 "parser.y"
                                                {
          (yyval.node) = createNode(EXTERN_METHOD_NODE, (char*)dtype_to_str((yyvsp[-5].data_type)));
          addChild((yyval.node), createNode(ID_NODE, (yyvsp[-4].text)));
          addChild((yyval.node), (yyvsp[-2].node));
      }
#line 1543 "parser.tab.c"
    break;

  case 6: /* extern_decl: EXTERN VOID ID '(' param_list_opt ')' ';'  */
#line 114 "parser.y"
                                                {
          (yyval.node) = createNode(EXTERN_METHOD_NODE, "void");
          addChild((yyval.node), createNode(ID_NODE, (yyvsp[-4].text)));
          addChild((yyval.node), (yyvsp[-2].node));
      }
#line 1553 "parser.tab.c"
    break;

  case 7: /* global_declarations: var_decl global_declarations  */
#line 122 "parser.y"
                                 {
        (yyval.node) = (yyvsp[0].node);
        prependChild((yyval.node), (yyvsp[-1].node));
    }
#line 1562 "parser.tab.c"
    break;

  case 8: /* global_declarations: method_decl method_decl_list  */
#line 126 "parser.y"
                                   {
        (yyval.node) = createNode(DECLARATIONS_NODE, NULL);
        addChild((yyval.node), (yyvsp[-1].node));
        for(int i=0; i<(yyvsp[0].node)->child_count; i++) {
            addChild((yyval.node), (yyvsp[0].node)->children[i]);
        }
        free((yyvsp[0].node)->children); free((yyvsp[0].node));
    }
#line 1575 "parser.tab.c"
    break;

  case 9: /* global_declarations: %empty  */
#line 134 "parser.y"
      { (yyval.node) = createNode(DECLARATIONS_NODE, NULL); }
#line 1581 "parser.tab.c"
    break;

  case 10: /* var_decl_list: var_decl_list var_decl  */
#line 141 "parser.y"
                           {
        (yyval.node) = (yyvsp[-1].node);
        addChild((yyval.node), (yyvsp[0].node));
    }
#line 1590 "parser.tab.c"
    break;

  case 11: /* var_decl_list: %empty  */
#line 145 "parser.y"
      { (yyval.node) = createNode(VAR_DECL_LIST_NODE, NULL); }
#line 1596 "parser.tab.c"
    break;

  case 12: /* var_decl: type ID id_list_tail ';'  */
#line 149 "parser.y"
                               {
          (yyval.node) = createNode(VAR_DECL_NODE, (char*)dtype_to_str((yyvsp[-3].data_type)));
          addChild((yyval.node), createNode(ID_NODE, (yyvsp[-2].text)));
          for(int i=0; i<(yyvsp[-1].node)->child_count; i++) {
              addChild((yyval.node), (yyvsp[-1].node)->children[i]);
          }
          free((yyvsp[-1].node)->children); free((yyvsp[-1].node));
      }
#line 1609 "parser.tab.c"
    break;

  case 13: /* id_list_tail: id_list_tail ',' ID  */
#line 161 "parser.y"
                        {
        (yyval.node) = (yyvsp[-2].node);
        addChild((yyval.node), createNode(ID_NODE, (yyvsp[0].text)));
    }
#line 1618 "parser.tab.c"
    break;

  case 14: /* id_list_tail: %empty  */
#line 165 "parser.y"
      { (yyval.node) = createNode(ID_LIST_NODE, NULL); }
#line 1624 "parser.tab.c"
    break;

  case 15: /* method_decl_list: method_decl_list method_decl  */
#line 172 "parser.y"
                                 {
        (yyval.node) = (yyvsp[-1].node);
        addChild((yyval.node), (yyvsp[0].node));
    }
#line 1633 "parser.tab.c"
    break;

  case 16: /* method_decl_list: %empty  */
#line 176 "parser.y"
      { (yyval.node) = createNode(METHOD_DECL_LIST_NODE, NULL); }
#line 1639 "parser.tab.c"
    break;

  case 17: /* method_decl: type ID '(' param_list_opt ')' block  */
#line 180 "parser.y"
                                           {
          (yyval.node) = createNode(METHOD_NODE, (char*)dtype_to_str((yyvsp[-5].data_type)));
          addChild((yyval.node), createNode(ID_NODE, (yyvsp[-4].text)));
          addChild((yyval.node), (yyvsp[-2].node));
          addChild((yyval.node), (yyvsp[0].node));
      }
#line 1650 "parser.tab.c"
    break;

  case 18: /* method_decl: VOID ID '(' param_list_opt ')' block  */
#line 186 "parser.y"
                                           {
          (yyval.node) = createNode(METHOD_NODE, "void");
          addChild((yyval.node), createNode(ID_NODE, (yyvsp[-4].text)));
          addChild((yyval.node), (yyvsp[-2].node));
          addChild((yyval.node), (yyvsp[0].node));
      }
#line 1661 "parser.tab.c"
    break;

  case 19: /* param_list: param  */
#line 196 "parser.y"
            {
          (yyval.node) = createNode(PARAM_LIST_NODE, NULL);
          addChild((yyval.node), (yyvsp[0].node));
      }
#line 1670 "parser.tab.c"
    break;

  case 20: /* param_list: param_list ',' param  */
#line 200 "parser.y"
                           {
          (yyval.node) = (yyvsp[-2].node);
          addChild((yyval.node), (yyvsp[0].node));
      }
#line 1679 "parser.tab.c"
    break;

  case 21: /* param_list_opt: param_list  */
#line 207 "parser.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1685 "parser.tab.c"
    break;

  case 22: /* param_list_opt: %empty  */
#line 208 "parser.y"
      { (yyval.node) = createNode(PARAM_LIST_NODE, "empty"); }
#line 1691 "parser.tab.c"
    break;

  case 23: /* param: type ID  */
#line 212 "parser.y"
              {
          (yyval.node) = createNode(PARAMETER_NODE, (char*)dtype_to_str((yyvsp[-1].data_type)));
          addChild((yyval.node), createNode(ID_NODE, (yyvsp[0].text)));
      }
#line 1700 "parser.tab.c"
    break;

  case 24: /* type: INT  */
#line 221 "parser.y"
          { (yyval.data_type) = TYPE_INT; }
#line 1706 "parser.tab.c"
    break;

  case 25: /* type: BOOLEAN  */
#line 222 "parser.y"
              { (yyval.data_type) = TYPE_BOOLEAN; }
#line 1712 "parser.tab.c"
    break;

  case 26: /* type: FLOAT  */
#line 223 "parser.y"
            { (yyval.data_type) = TYPE_FLOAT; }
#line 1718 "parser.tab.c"
    break;

  case 27: /* block: '{' var_decl_list statement_list '}'  */
#line 229 "parser.y"
                                           {
          (yyval.node) = createNode(BLOCK_NODE, NULL);
          addChild((yyval.node), (yyvsp[-2].node));
          addChild((yyval.node), (yyvsp[-1].node));
      }
#line 1728 "parser.tab.c"
    break;

  case 28: /* statement_list: statement_list statement  */
#line 239 "parser.y"
                             {
        (yyval.node) = (yyvsp[-1].node);
        if ((yyvsp[0].node) != NULL) addChild((yyval.node), (yyvsp[0].node));
    }
#line 1737 "parser.tab.c"
    break;

  case 29: /* statement_list: %empty  */
#line 243 "parser.y"
      { (yyval.node) = createNode(STATEMENT_LIST_NODE, NULL); }
#line 1743 "parser.tab.c"
    break;

  case 30: /* statement: ID '=' expr ';'  */
#line 247 "parser.y"
                      {
          (yyval.node) = createNode(ASSIGNMENT_NODE, NULL);
          addChild((yyval.node), createNode(ID_NODE, (yyvsp[-3].text)));
          addChild((yyval.node), (yyvsp[-1].node));
      }
#line 1753 "parser.tab.c"
    break;

  case 31: /* statement: method_call ';'  */
#line 252 "parser.y"
                      {
          (yyval.node) = (yyvsp[-1].node);
      }
#line 1761 "parser.tab.c"
    break;

  case 32: /* statement: IF '(' expr ')' block  */
#line 255 "parser.y"
                            {
          (yyval.node) = createNode(IF_NODE, NULL);
          addChild((yyval.node), (yyvsp[-2].node));
          addChild((yyval.node), (yyvsp[0].node));
      }
#line 1771 "parser.tab.c"
    break;

  case 33: /* statement: IF '(' expr ')' block ELSE block  */
#line 260 "parser.y"
                                       {
          (yyval.node) = createNode(IF_NODE, "else");
          addChild((yyval.node), (yyvsp[-4].node));
          addChild((yyval.node), (yyvsp[-2].node));
          addChild((yyval.node), (yyvsp[0].node));
      }
#line 1782 "parser.tab.c"
    break;

  case 34: /* statement: WHILE '(' expr ')' block  */
#line 266 "parser.y"
                               {
          (yyval.node) = createNode(WHILE_NODE, NULL);
          addChild((yyval.node), (yyvsp[-2].node));
          addChild((yyval.node), (yyvsp[0].node));
      }
#line 1792 "parser.tab.c"
    break;

  case 35: /* statement: RETURN expr_opt ';'  */
#line 271 "parser.y"
                          {
          (yyval.node) = createNode(RETURN_NODE, NULL);
          if ((yyvsp[-1].node) != NULL) addChild((yyval.node), (yyvsp[-1].node));
      }
#line 1801 "parser.tab.c"
    break;

  case 36: /* statement: ';'  */
#line 275 "parser.y"
          { (yyval.node) = NULL; }
#line 1807 "parser.tab.c"
    break;

  case 37: /* statement: block  */
#line 276 "parser.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1813 "parser.tab.c"
    break;

  case 38: /* expr_opt: expr  */
#line 280 "parser.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 1819 "parser.tab.c"
    break;

  case 39: /* expr_opt: %empty  */
#line 281 "parser.y"
      { (yyval.node) = NULL; }
#line 1825 "parser.tab.c"
    break;

  case 40: /* method_call: ID '(' args_opt ')'  */
#line 285 "parser.y"
                          {
          (yyval.node) = createNode(CALL_NODE, (yyvsp[-3].text));
          if ((yyvsp[-1].node) != NULL) {
              addChild((yyval.node), (yyvsp[-1].node));
          }
      }
#line 1836 "parser.tab.c"
    break;

  case 41: /* args_opt: arg_list  */
#line 294 "parser.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 1842 "parser.tab.c"
    break;

  case 42: /* args_opt: %empty  */
#line 295 "parser.y"
      { (yyval.node) = createNode(ARG_LIST_NODE, "empty"); }
#line 1848 "parser.tab.c"
    break;

  case 43: /* arg_list: expr  */
#line 299 "parser.y"
           {
          (yyval.node) = createNode(ARG_LIST_NODE, NULL);
          addChild((yyval.node), (yyvsp[0].node));
      }
#line 1857 "parser.tab.c"
    break;

  case 44: /* arg_list: arg_list ',' expr  */
#line 303 "parser.y"
                        {
          (yyval.node) = (yyvsp[-2].node);
          addChild((yyval.node), (yyvsp[0].node));
      }
#line 1866 "parser.tab.c"
    break;

  case 45: /* expr: ID  */
#line 312 "parser.y"
         { (yyval.node) = createNode(ID_NODE, (yyvsp[0].text)); }
#line 1872 "parser.tab.c"
    break;

  case 46: /* expr: method_call  */
#line 313 "parser.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 1878 "parser.tab.c"
    break;

  case 47: /* expr: literal  */
#line 314 "parser.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1884 "parser.tab.c"
    break;

  case 48: /* expr: expr '+' expr  */
#line 315 "parser.y"
                    { (yyval.node) = createNode(OPERATION_NODE, "+"); addChild((yyval.node), (yyvsp[-2].node)); addChild((yyval.node), (yyvsp[0].node)); }
#line 1890 "parser.tab.c"
    break;

  case 49: /* expr: expr '-' expr  */
#line 316 "parser.y"
                    { (yyval.node) = createNode(OPERATION_NODE, "-"); addChild((yyval.node), (yyvsp[-2].node)); addChild((yyval.node), (yyvsp[0].node)); }
#line 1896 "parser.tab.c"
    break;

  case 50: /* expr: expr '*' expr  */
#line 317 "parser.y"
                    { (yyval.node) = createNode(OPERATION_NODE, "*"); addChild((yyval.node), (yyvsp[-2].node)); addChild((yyval.node), (yyvsp[0].node)); }
#line 1902 "parser.tab.c"
    break;

  case 51: /* expr: expr '/' expr  */
#line 318 "parser.y"
                    { (yyval.node) = createNode(OPERATION_NODE, "/"); addChild((yyval.node), (yyvsp[-2].node)); addChild((yyval.node), (yyvsp[0].node)); }
#line 1908 "parser.tab.c"
    break;

  case 52: /* expr: expr '%' expr  */
#line 319 "parser.y"
                    { (yyval.node) = createNode(OPERATION_NODE, "%"); addChild((yyval.node), (yyvsp[-2].node)); addChild((yyval.node), (yyvsp[0].node)); }
#line 1914 "parser.tab.c"
    break;

  case 53: /* expr: expr '<' expr  */
#line 320 "parser.y"
                    { (yyval.node) = createNode(OPERATION_NODE, "<"); addChild((yyval.node), (yyvsp[-2].node)); addChild((yyval.node), (yyvsp[0].node)); }
#line 1920 "parser.tab.c"
    break;

  case 54: /* expr: expr '>' expr  */
#line 321 "parser.y"
                    { (yyval.node) = createNode(OPERATION_NODE, ">"); addChild((yyval.node), (yyvsp[-2].node)); addChild((yyval.node), (yyvsp[0].node)); }
#line 1926 "parser.tab.c"
    break;

  case 55: /* expr: expr EQUALS expr  */
#line 322 "parser.y"
                       { (yyval.node) = createNode(OPERATION_NODE, "=="); addChild((yyval.node), (yyvsp[-2].node)); addChild((yyval.node), (yyvsp[0].node)); }
#line 1932 "parser.tab.c"
    break;

  case 56: /* expr: expr AND expr  */
#line 323 "parser.y"
                    { (yyval.node) = createNode(OPERATION_NODE, "&&"); addChild((yyval.node), (yyvsp[-2].node)); addChild((yyval.node), (yyvsp[0].node)); }
#line 1938 "parser.tab.c"
    break;

  case 57: /* expr: expr OR expr  */
#line 324 "parser.y"
                   { (yyval.node) = createNode(OPERATION_NODE, "||"); addChild((yyval.node), (yyvsp[-2].node)); addChild((yyval.node), (yyvsp[0].node)); }
#line 1944 "parser.tab.c"
    break;

  case 58: /* expr: '-' expr  */
#line 325 "parser.y"
                            { (yyval.node) = createNode(OPERATION_NODE, "- (unary)"); addChild((yyval.node), (yyvsp[0].node)); }
#line 1950 "parser.tab.c"
    break;

  case 59: /* expr: NOT expr  */
#line 326 "parser.y"
               { (yyval.node) = createNode(OPERATION_NODE, "!"); addChild((yyval.node), (yyvsp[0].node)); }
#line 1956 "parser.tab.c"
    break;

  case 60: /* expr: '(' expr ')'  */
#line 327 "parser.y"
                   { (yyval.node) = (yyvsp[-1].node); }
#line 1962 "parser.tab.c"
    break;

  case 61: /* literal: INT_LITERAL  */
#line 331 "parser.y"
                  { (yyval.node) = createNode(LITERAL_NODE, (yyvsp[0].text)); }
#line 1968 "parser.tab.c"
    break;

  case 62: /* literal: FLOAT_LITERAL  */
#line 332 "parser.y"
                    { (yyval.node) = createNode(LITERAL_NODE, (yyvsp[0].text)); }
#line 1974 "parser.tab.c"
    break;

  case 63: /* literal: TRUE  */
#line 333 "parser.y"
           { (yyval.node) = createNode(LITERAL_NODE, "true"); }
#line 1980 "parser.tab.c"
    break;

  case 64: /* literal: FALSE  */
#line 334 "parser.y"
            { (yyval.node) = createNode(LITERAL_NODE, "false"); }
#line 1986 "parser.tab.c"
    break;


#line 1990 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 337 "parser.y"


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
