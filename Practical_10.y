%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>  /* For pow() function */
int yylex();
void yyerror(const char *s);
%}

%union {
    double val;  /* Use double to handle division and exponentiation results */
}

%token <val> DIGIT
%token PLUS MINUS MULT DIV EXP LPAREN RPAREN NEWLINE

%type <val> L E T F G

%%

L: E NEWLINE  { printf("Result: %.2f\n", $1); return 0; }
 ;

E: E PLUS T   { $$ = $1 + $3; }    /* E.val = E.val + T.val */
 | E MINUS T  { $$ = $1 - $3; }    /* E.val = E.val - T.val */
 | T          { $$ = $1; }         /* E.val = T.val */
 ;

T: T MULT F   { $$ = $1 * $3; }    /* T.val = T.val * F.val */
 | T DIV F    { if ($3 == 0) { yyerror("Division by zero"); return 1; } 
                $$ = $1 / $3; }   /* T.val = T.val / F.val */
 | F          { $$ = $1; }         /* T.val = F.val */
 ;

F: G EXP F    { $$ = pow($1, $3); } /* F.val = G.val ^ F.val */
 | G          { $$ = $1; }         /* F.val = G.val */
 ;

G: LPAREN E RPAREN { $$ = $2; }    /* G.val = E.val */
 | DIGIT          { $$ = $1; }     /* G.val = digit.lexval */
 ;

%%

/* Simple lexer */
int yylex() {
    int c = getchar();
    switch (c) {
        case '+': return PLUS;
        case '-': return MINUS;
        case '*': return MULT;
        case '/': return DIV;
        case '^': return EXP;
        case '(': return LPAREN;
        case ')': return RPAREN;
        case '\n': return NEWLINE;
        case ' ': case '\t': return yylex(); /* Skip whitespace */
        default:
            if (c >= '0' && c <= '9') {
                yylval.val = c - '0';  /* Convert char to integer */
                return DIGIT;
            }
            return c;  /* Return unrecognized characters */
    }
}

void yyerror(const char *s) {
    printf("Invalid expression: %s\n", s);
}

int main() {
    printf("Enter an arithmetic expression: ");
    return yyparse();
}