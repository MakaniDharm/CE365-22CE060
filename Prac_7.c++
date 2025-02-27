#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <string>

using namespace std;

class FirstFollow
{
private:
  map<char, set<char>> first;
  map<char, set<char>> follow;
  map<char, vector<string>> productions;
  char startSymbol;

  void computeFirst(char symbol)
  {
    if (!first[symbol].empty())
      return; 

    for (const string &rule : productions[symbol])
    {
      char firstChar = rule[0];

      if (islower(firstChar) || firstChar == '(' || firstChar == ')')
      { 
        first[symbol].insert(firstChar);
      }
      else
      {
        computeFirst(firstChar);
        first[symbol].insert(first[firstChar].begin(), first[firstChar].end());

        if (first[firstChar].find('e') != first[firstChar].end())
        {
          first[symbol].insert('e');
        }
      }
    }
  }

  void computeFollow(char symbol)
  {
    if (!follow[symbol].empty())
      return;

    if (symbol == startSymbol)
      follow[symbol].insert('$'); 

    for (auto &[lhs, rules] : productions)
    {
      for (const string &rule : rules)
      {
        for (size_t i = 0; i < rule.length(); i++)
        {
          if (rule[i] == symbol)
          {
            bool epsilonNext = true;
            for (size_t j = i + 1; j < rule.length(); j++)
            {
              char next = rule[j];
              if (!isupper(next))
              {
                follow[symbol].insert(next);
                epsilonNext = false;
                break;
              }
              else
              {
                follow[symbol].insert(first[next].begin(), first[next].end());
                if (first[next].find('e') == first[next].end())
                {
                  epsilonNext = false;
                  break;
                }
              }
            }
            if (epsilonNext || i + 1 == rule.length())
            {
              computeFollow(lhs);
              follow[symbol].insert(follow[lhs].begin(), follow[lhs].end());
            }
          }
        }
      }
    }
  }

public:
  FirstFollow()
  {
    productions = {
        {'S', {"ABC", "D"}},
        {'A', {"a", "e"}},
        {'B', {"b", "e"}},
        {'C', {"(S)", "c"}},
        {'D', {"AC"}}};
    startSymbol = 'S';
  }

  void computeFirstFollow()
  {
    for (auto &[symbol, _] : productions)
    {
      computeFirst(symbol);
    }
    for (auto &[symbol, _] : productions)
    {
      computeFollow(symbol);
    }
  }

  void printResults()
  {
    for (auto &[symbol, firstSet] : first)
    {
      cout << "First(" << symbol << ") = { ";
      for (char ch : firstSet)
        cout << ch << " ";
      cout << "}\n";
    }
    cout << endl;
    for (auto &[symbol, followSet] : follow)
    {
      cout << "Follow(" << symbol << ") = { ";
      for (char ch : followSet)
        cout << ch << " ";
      cout << "}\n";
    }
  }
};

int main()
{
  FirstFollow parser;
  parser.computeFirstFollow();
  parser.printResults();
  return 0;
}
//g++ -std=c++17 Prac_7.c++ -o output
// ./output`