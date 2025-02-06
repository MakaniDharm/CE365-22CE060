#include<stdio.h>
#include<string.h>

#define Max_States 100
#define Max_Symbols 10

int main(){
  int num_symbols,num_states,num_accepting_states;
  char symbols[Max_Symbols];
  int transition_table[Max_States][Max_Symbols];
  int start_state,accepting_state[Max_States];
  char input[100];

  printf("Enter no. of Symbols:");
  scanf("%d",&num_symbols);

  printf("Input Symbols:");
  for(int i=0;i<num_symbols;i++){
    scanf("%s",&symbols[i]);
  }

  printf("enter no of States:");
  scanf("%d",&num_states);

  printf("Enter Initial States:");
  scanf("%d",&start_state);

  
  printf("Number of accepting states: ");
  scanf("%d", &num_accepting_states);
  printf("Accepting states: ");
  for (int i = 0; i < num_accepting_states; i++) {
    scanf("%d", &accepting_state[i]);
  }
  
  for (int i = 0; i < num_states; i++) {
   for (int j = 0; j < num_symbols; j++) {
    scanf("%d",&transition_table[i][j]);
    }
  }

}