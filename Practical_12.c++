#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <cctype>
#include <cmath>

using namespace std;

// Structure for tokens in the expression
struct Token
{
  enum Type
  {
    NUMBER,
    VARIABLE,
    OPERATOR,
    LEFT_PAREN,
    RIGHT_PAREN
  };
  Type type;
  string value;

  Token(Type t, string v) : type(t), value(v) {}
};

// Check if character is an operator
bool isOperator(char c)
{
  return c == '+' || c == '-' || c == '*' || c == '/';
}

// Check if token is a number
bool isNumber(const Token &token)
{
  if (token.type != Token::NUMBER)
    return false;

  // Check if the token value can be converted to a number
  try
  {
    stod(token.value);
    return true;
  }
  catch (...)
  {
    return false;
  }
}

// Calculate result of simple operation
double calculate(double a, double b, char op)
{
  switch (op)
  {
  case '+':
    return a + b;
  case '-':
    return a - b;
  case '*':
    return a * b;
  case '/':
    return a / b;
  default:
    return 0;
  }
}

// Format number (remove trailing zeros for whole numbers)
string formatNumber(double num)
{
  // Check if it's a whole number
  if (num == static_cast<int>(num))
  {
    return to_string(static_cast<int>(num));
  }

  string result = to_string(num);
  // Remove trailing zeros
  result.erase(result.find_last_not_of('0') + 1);
  // Remove decimal point if it's the last character
  if (result.back() == '.')
  {
    result.pop_back();
  }
  return result;
}

// Tokenize the input expression
vector<Token> tokenize(const string &expr)
{
  vector<Token> tokens;

  for (size_t i = 0; i < expr.length(); i++)
  {
    char c = expr[i];

    // Skip spaces
    if (isspace(c))
      continue;

    // Handle numbers
    if (isdigit(c) || (c == '.' && i + 1 < expr.length() && isdigit(expr[i + 1])))
    {
      string number;
      while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.'))
      {
        number += expr[i++];
      }
      i--; // Adjust for the loop increment
      tokens.push_back(Token(Token::NUMBER, number));
    }
    // Handle variables
    else if (isalpha(c) || c == '_')
    {
      string variable;
      while (i < expr.length() && (isalnum(expr[i]) || expr[i] == '_'))
      {
        variable += expr[i++];
      }
      i--; // Adjust for the loop increment
      tokens.push_back(Token(Token::VARIABLE, variable));
    }
    // Handle operators and parentheses
    else if (isOperator(c))
    {
      tokens.push_back(Token(Token::OPERATOR, string(1, c)));
    }
    else if (c == '(')
    {
      tokens.push_back(Token(Token::LEFT_PAREN, "("));
    }
    else if (c == ')')
    {
      tokens.push_back(Token(Token::RIGHT_PAREN, ")"));
    }
  }

  return tokens;
}

// Apply constant folding to a vector of tokens
vector<Token> applyConstantFolding(const vector<Token> &tokens)
{
  vector<Token> result = tokens;
  bool changed = true;

  // Continue folding until no more changes can be made
  while (changed)
  {
    changed = false;

    // Find parenthesized expressions first
    for (size_t i = 0; i < result.size(); i++)
    {
      if (result[i].type == Token::LEFT_PAREN)
      {
        // Find matching closing parenthesis
        int parenCount = 1;
        size_t j = i + 1;

        while (j < result.size() && parenCount > 0)
        {
          if (result[j].type == Token::LEFT_PAREN)
            parenCount++;
          if (result[j].type == Token::RIGHT_PAREN)
            parenCount--;
          j++;
        }

        if (parenCount == 0)
        {
          // Check if expression inside parens is all constants
          bool allConstants = true;
          bool hasOperator = false;

          for (size_t k = i + 1; k < j - 1; k++)
          {
            if (result[k].type == Token::VARIABLE)
            {
              allConstants = false;
              break;
            }
            if (result[k].type == Token::OPERATOR)
            {
              hasOperator = true;
            }
          }

          // If we have all constants and at least one operator
          if (allConstants && hasOperator)
          {
            // Evaluate the constant expression
            double value = 0;
            char op = '+'; // Default operator
            bool firstNum = true;

            for (size_t k = i + 1; k < j - 1; k++)
            {
              if (result[k].type == Token::NUMBER)
              {
                double num = stod(result[k].value);
                if (firstNum)
                {
                  value = num;
                  firstNum = false;
                }
                else
                {
                  value = calculate(value, num, op);
                }
              }
              else if (result[k].type == Token::OPERATOR)
              {
                op = result[k].value[0];
              }
            }

            // Replace the entire parenthesized expression with result
            result[i] = Token(Token::NUMBER, formatNumber(value));
            result.erase(result.begin() + i + 1, result.begin() + j);
            changed = true;
            break;
          }
        }
      }
    }

    if (changed)
      continue;

    // Find simple NUMBER OPERATOR NUMBER patterns
    for (size_t i = 0; i + 2 < result.size(); i++)
    {
      if (isNumber(result[i]) &&
          result[i + 1].type == Token::OPERATOR &&
          isNumber(result[i + 2]))
      {

        double a = stod(result[i].value);
        char op = result[i + 1].value[0];
        double b = stod(result[i + 2].value);
        double value = calculate(a, b, op);

        // Replace the pattern with result
        result[i] = Token(Token::NUMBER, formatNumber(value));
        result.erase(result.begin() + i + 1, result.begin() + i + 3);
        changed = true;
        break;
      }
    }
  }

  return result;
}

// Convert tokens back to string representation
string tokensToString(const vector<Token> &tokens)
{
  string result;

  for (size_t i = 0; i < tokens.size(); i++)
  {
    if (i > 0 &&
        tokens[i - 1].type != Token::LEFT_PAREN &&
        tokens[i].type != Token::RIGHT_PAREN &&
        tokens[i].type != Token::OPERATOR &&
        tokens[i - 1].type != Token::OPERATOR)
    {
      result += " ";
    }

    result += tokens[i].value;
  }

  return result;
}

// Main function to optimize an expression
string optimizeExpression(const string &expr)
{
  vector<Token> tokens = tokenize(expr);
  vector<Token> optimized = applyConstantFolding(tokens);
  return tokensToString(optimized);
}

int main()
{
  string expression;
  cout << "Enter an arithmetic expression: ";
  getline(cin, expression);

  string optimized = optimizeExpression(expression);
  cout << "Optimized expression: " << optimized << endl;

  return 0;
}