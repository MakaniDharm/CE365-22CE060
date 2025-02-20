%{
#include <stdio.h>
#include <stdlib.h>
%}

%%
Charusat   printf("university");
.          putchar(yytext[0]);
\n         return 0;
%%

int main() {
    printf("Enter a string:");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
