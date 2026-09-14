%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
%}

%option yylineno

digit    [0-9]
alpha     [a-zA-Z]
alpha_num      ({alpha}|{digit}|_)
float_literal  {digit}+\.{digit}+
int_literal    {digit}+
id     {alpha}{alpha_num}*


%%

/* Reserved words */

"int"    { return INT; }
"boolean"   { return BOOLEAN; }
"float"    { return FLOAT; }
"void"   { return VOID; }
"if" { return IF; }
"else" { return ELSE; }
"while" { return WHILE; }
"return"   { return RETURN; }
"true"   { return TRUE; }
"false"   { return FALSE; }


/* Arithmetic operators */

"+"   { return '+'; }
"*"   { return '*'; }
"-"   { return '-'; }
"/"   { return '/'; }
"%"   { return '%'; }

/* Logical operators */

"&&"   { return AND; }
"||"   { return OR; }
"!"    { return NOT; }

/* Comparative operators */

"<"    { return '<'; }
">"    { return '>'; }
"=="   { return EQUALS;}

/* Delimiters */

";"   { return ';'; }
"("   { return '('; }
")"   { return ')'; }
"}"   { return '}'; }
"{"   { return '{'; }

/* Other simbols */

"="   { return '='; }
","   { return ','; }

/* ID and literals */

{id}       { yylval.text = strdup(yytext); return ID; }
{int_literal} {
    long long valor = strtoll(yytext, NULL, 10);

    if (valor > 2147483647) {
        printf("Lexic error in line %d: out of bounds Integer '%s'\n",
               yylineno, yytext);
    } else {
        yylval.text = strdup(yytext);
        return INT_LITERAL;
    }
}
{float_literal}     { yylval.text = strdup(yytext); return FLOAT_LITERAL; }

/* Comments */

"//".*    ;
"/*"([^*]|\*+[^*/])*\*+"/" ;

/* Whitespace */

[ \t\n]+      ; /* Ignores empty spaces and line jumps */
. {
    printf("Lexic error in line %d: Unrecognized symbol '%s'\n",
           yylineno, yytext);
}
%%
