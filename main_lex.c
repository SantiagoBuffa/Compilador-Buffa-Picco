#include <stdio.h>
#include "parser.tab.h"

YYSTYPE yylval;

int yylex(void);

int main() {
    int token;

    while ((token = yylex()) != 0) {
        printf("Token: %d\n", token);
    }

    return 0;
}

int yywrap(void) {
    return 1;
}