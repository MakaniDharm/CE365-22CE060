#include<stdio.h>
#include<string.h>

int main(){
  
  char input[1000];
  int i=0,len;

  printf("Enter a String:");
  gets(input);

  len=strlen(input);

  if(input == NULL)
  {
    printf("Invalid String");
  }

//takes a
  while(i<len && input[i]=='a'){
    i++;
  }
// For b
  if(i<len && input[i]=='b' && i+1<len && input[i+1]=='b' && i+2==len){
    printf("Valid String\n");
  }
  else{
    printf("Invalid String\n");
  }
}