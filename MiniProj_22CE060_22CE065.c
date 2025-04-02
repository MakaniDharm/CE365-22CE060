#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isDataType(char token[])
{
  char dataTypes[][10] = {"int", "float", "double", "char", "void"};
  for (int i = 0; i < 5; i++)
  {
    if (strcmp(token, dataTypes[i]) == 0)
      return 1;
  }
  return 0;
}

int isValidIdentifier(char token[])
{
  if (!isalpha(token[0]) && token[0] != '_')
    return 0;
  for (int i = 1; i < strlen(token); i++)
  {
    if (!isalnum(token[i]) && token[i] != '_')
      return 0;
  }
  return 1;
}

int isValidValue(char type[], char value[])
{
  int len = strlen(value), hasDot = 0;

  if (strcmp(type, "int") == 0)
  {
    for (int i = 0; i < len; i++)
    {
      if (!isdigit(value[i]))
        return 0;
    }
    return 1;
  }

  if (strcmp(type, "float") == 0 || strcmp(type, "double") == 0)
  {
    for (int i = 0; i < len; i++)
    {
      if (value[i] == '.')
        hasDot++;
      else if (!isdigit(value[i]))
        return 0;
    }
    return (hasDot == 1);
  }

  if (strcmp(type, "char") == 0)
  {
    return (len == 3 && value[0] == '\'' && value[2] == '\'');
  }

  return 0;
}

void analyzeDeclaration(char input[])
{
  char tokens[10][50];
  int tokenCount = 0, j = 0;

  // Simplified tokenization: handle input character by character
  for (int i = 0; input[i] != '\0'; i++)
  {
    if (input[i] == ' ' || input[i] == '=' || input[i] == ',' || input[i] == ';')
    {
      if (j > 0)
      { // End of a token
        tokens[tokenCount][j] = '\0';
        tokenCount++;
        j = 0;
      }
      if (input[i] == '=')
      { // Treat '=' as a separate token
        tokens[tokenCount][0] = '=';
        tokens[tokenCount][1] = '\0';
        tokenCount++;
      }
    }
    else
    {
      tokens[tokenCount][j] = input[i];
      j++;
    }
  }
  if (j > 0)
  { // Handle the last token
    tokens[tokenCount][j] = '\0';
    tokenCount++;
  }

  // Analyze tokens
  if (tokenCount >= 2 && isDataType(tokens[0]))
  {
    int i = 1;
    while (i < tokenCount)
    {
      if (!isValidIdentifier(tokens[i]))
      {
        printf("Invalid declaration: %s\n", input);
        return;
      }
      if (i + 1 < tokenCount && strcmp(tokens[i + 1], "=") == 0)
      {
        if (i + 2 < tokenCount && isValidValue(tokens[0], tokens[i + 2]))
        {
          i += 3; // Skip identifier, '=', and value
        }
        else
        {
          printf("Invalid declaration: %s\n", input);
          return;
        }
      }
      else
      {
        i++; // Move to next token
      }
    }
    printf("Valid declaration: %s\n", input);
  }
  else
  {
    printf("Invalid declaration: %s\n", input);
  }
}

int main()
{
  char input[100];
  printf("Enter a variable declaration: ");
  fgets(input, sizeof(input), stdin);
  input[strcspn(input, "\n")] = 0; // Remove newline
  analyzeDeclaration(input);
  return 0;
}