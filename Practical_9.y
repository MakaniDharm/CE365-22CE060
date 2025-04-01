%{
#include <stdio.h>
int yylex();
void yyerror(const char *s);
int input_char;
int valid = 0;  /* Flag to track if string is valid */
%}

%token I T E_CHAR A B

%%

Start: S '\n'  { valid = 1; printf("Valid string\n"); return 0; }
     ;

S: I E T S Sp
 | A
 ;

Sp: E_CHAR S
  | /* epsilon */
  ;

E: B
 ;

%%

/* Simple character-based lexer */
int yylex() {
    input_char = getchar();
    if (input_char == 'i') return I;
    if (input_char == 't') return T;
    if (input_char == 'e') return E_CHAR;
    if (input_char == 'a') return A;
    if (input_char == 'b') return B;
    if (input_char == '\n') return '\n';
    return input_char;
}

void yyerror(const char *s) {
    if (!valid) {
        printf("Invalid string\n");
    }
}

int main() {
    printf("Enter string to validate: ");
    yyparse();
    return 0;
}