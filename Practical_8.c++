#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
using namespace std;

struct Production {
    string lhs;
    string rhs;
};

bool isNonTerminal(char c) {
    return (c >= 'A' && c <= 'Z');
}

bool isLL1(const vector<Production>& grammar, 
           const map<char, set<char> >& first, 
           const map<char, set<char> >& follow) {
    vector<set<char> > firstSets;
    for (char nt = 'A'; nt <= 'Z'; nt++) {
        firstSets.clear();
        for (size_t i = 0; i < grammar.size(); i++) {
            if (grammar[i].lhs[0] != nt) continue;
            set<char> firstOfProd;
            char firstSym = grammar[i].rhs[0];
            if (firstSym == '#') {
                firstOfProd.insert('#');
            } else if (isNonTerminal(firstSym)) {
                firstOfProd = first.at(firstSym);
            } else {
                firstOfProd.insert(firstSym);
            }
            firstSets.push_back(firstOfProd);
        }
        for (size_t i = 0; i < firstSets.size(); i++) {
            for (size_t j = i + 1; j < firstSets.size(); j++) {
                set<char> intersection;
                for (set<char>::iterator it = firstSets[i].begin(); it != firstSets[i].end(); ++it) {
                    if (firstSets[j].find(*it) != firstSets[j].end()) {
                        intersection.insert(*it);
                    }
                }
                if (!intersection.empty() && intersection.find('#') == intersection.end()) {
                    return false;
                }
            }
        }
        for (size_t i = 0; i < firstSets.size(); i++) {
            if (firstSets[i].find('#') != firstSets[i].end()) {
                set<char> intersection;
                for (set<char>::iterator it = first.at(nt).begin(); it != first.at(nt).end(); ++it) {
                    if (follow.at(nt).find(*it) != follow.at(nt).end()) {
                        intersection.insert(*it);
                    }
                }
                if (!intersection.empty()) {
                    return false;
                }
            }
        }
    }
    return true;
}

map<pair<char, char>, string> constructParsingTable(
    const vector<Production>& grammar, 
    const map<char, set<char> >& first, 
    const map<char, set<char> >& follow, 
    const set<char>& terminals, 
    const set<char>& nonTerminals) {
    map<pair<char, char>, string> parsingTable;
    for (set<char>::iterator nt = nonTerminals.begin(); nt != nonTerminals.end(); ++nt) {
        for (set<char>::iterator t = terminals.begin(); t != terminals.end(); ++t) {
            parsingTable[make_pair(*nt, *t)] = "";
        }
    }
    for (size_t i = 0; i < grammar.size(); i++) {
        char nt = grammar[i].lhs[0];
        string rhs = grammar[i].rhs;
        char firstSym = rhs[0];
        set<char> firstOfProd;
        if (firstSym == '#') {
            firstOfProd.insert('#');
        } else if (isNonTerminal(firstSym)) {
            firstOfProd = first.at(firstSym);
        } else {
            firstOfProd.insert(firstSym);
        }
        for (set<char>::iterator t = firstOfProd.begin(); t != firstOfProd.end(); ++t) {
            if (*t != '#') {
                parsingTable[make_pair(nt, *t)] = rhs;
            }
        }
        if (firstOfProd.find('#') != firstOfProd.end()) {
            for (set<char>::iterator t = follow.at(nt).begin(); t != follow.at(nt).end(); ++t) {
                parsingTable[make_pair(nt, *t)] = rhs;
            }
        }
    }
    return parsingTable;
}

bool validateString(string input, 
                    const map<pair<char, char>, string>& parsingTable, 
                    char startSymbol, 
                    const set<char>& terminals) {
    // Clean the input string by removing non-terminal characters (e.g., parentheses)
    string cleanedInput;
    for (size_t i = 0; i < input.length(); i++) {
        if (terminals.find(input[i]) != terminals.end() && input[i] != '$') {
            cleanedInput += input[i];
        }
    }
    string stack = "$";
    stack += startSymbol;
    string inputStr = cleanedInput + "$";
    size_t inputPos = 0;
    while (!stack.empty()) {
        char top = stack.back();
        char current = inputStr[inputPos];
        if (top == '$' && current == '$') {
            return true;
        }
        if (top == '$' || current == '$') {
            return false;
        }
        if (terminals.find(top) != terminals.end()) {
            if (top == current) {
                stack.pop_back();
                inputPos++;
            } else {
                return false;
            }
        } else {
            // Check if the parsing table has an entry for the current non-terminal and terminal
            pair<char, char> key = make_pair(top, current);
            if (parsingTable.find(key) == parsingTable.end() || parsingTable.at(key).empty()) {
                return false; // No valid production, so the string is invalid
            }
            string prod = parsingTable.at(key);
            stack.pop_back();
            if (prod != "#") {
                for (int i = prod.size() - 1; i >= 0; i--) {
                    stack += prod[i];
                }
            }
        }
    }
    return false;
}

int main() {
    vector<Production> grammar = {
        {"S", "aA"},
        {"S", "bB"},
        {"A", "c"},
        {"A", "e"},
        {"B", "d"},
        {"B", "e"}
    };
    set<char> terminals = {'a', 'b', 'c', 'd', '$'};
    set<char> nonTerminals = {'S', 'A', 'B'};
    map<char, set<char> > first = {
        {'S', {'a', 'b'}},
        {'A', {'c', 'e'}},
        {'B', {'d', 'e'}}
    };
    map<char, set<char> > follow = {
        {'S', {'$'}},
        {'A', {'$'}},
        {'B', {'$'}}
    };
    bool isLL1Grammar = isLL1(grammar, first, follow);
    cout << "Grammar is " << (isLL1Grammar ? "LL(1)" : "not LL(1)") << endl;
    auto parsingTable = constructParsingTable(grammar, first, follow, terminals, nonTerminals);
    cout << "\nPredictive Parsing Table:\n";
    cout << "\t";
    for (set<char>::iterator t = terminals.begin(); t != terminals.end(); ++t) {
        cout << *t << "\t";
    }
    cout << endl;
    for (set<char>::iterator nt = nonTerminals.begin(); nt != nonTerminals.end(); ++nt) {
        cout << *nt << "\t";
        for (set<char>::iterator t = terminals.begin(); t != terminals.end(); ++t) {
            string prod = parsingTable[make_pair(*nt, *t)];
            if (prod.empty()) {
                cout << "-\t";
            } else {
                cout << *nt << "->" << prod << "\t";
            }
        }
        cout << endl;
    }
    string input;
    cout << "\nEnter the input string to validate : ";
    cin >> input;
    bool isValid = validateString(input, parsingTable, 'S', terminals);
    cout << (isValid ? "Valid string" : "Invalid string") << endl;
    return 0;
}

