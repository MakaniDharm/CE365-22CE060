
%{
    #include<stdio.h>
    int d=0,lc=0,uc=0,ec=0,ss=0,l=0;
%}
%%
[0-9] {d++;l++;}
[a-z] {lc++;l++;}
[A-Z] {uc++;l++;}
[*,;#$@] {ss++;l++;}
[\n] {if(d>0 && lc>0 && uc>0 && ss>0 && l>=9 && l<=15 && ec==0){
    printf("valid ");
}
else{
    printf("invalid ");
    d=0,lc=0,uc=0,ec=0,ss=0,l=0;
}
}
.     {ec++;} 
%%
int main()
{
    printf("Enter String : " );
    yylex();
    d=0,lc=0,uc=0,ec=0,ss=0,l=0;
    return 0;

}
int yywrap(){

}
