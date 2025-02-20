%{
#include <stdio.h>
int c=0, w=0, l=0;
%}

%option noyywrap 

%%

[^\n]  { c++; }   
\n     { c++; l++; } 
[ \t]+ { w++; } 

%%

int main() {
    printf("Enter text:\n");
    yylex(); 
    printf("Characters: %d\nWords: %d\nLines: %d\n", c, w+1, l); 
    return 0;
}