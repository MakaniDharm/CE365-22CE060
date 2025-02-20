#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cctype>

using namespace std;

// Keywords in C
unordered_set<string> keywords = {
    "auto", "break", "case", "char", "const", "continue", "default", "do", "double",
    "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register",
    "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef",
    "union", "unsigned", "void", "volatile", "while"
};

// Punctuation and operators in C
unordered_set<char> punctuation = { '(', ')', '{', '}', '[', ']', ',', ';', ':', '.' };
unordered_set<string> operators = { "+", "-", "*", "/", "%", "=", "==", "!=", "<", ">", "<=", ">=", "&&", "||", "!", "&", "|", "^", "~", "<<", ">>" };

// Symbol table
vector<string> symbolTable;

// Function to check if a string is a keyword
bool isKeyword(const string& str) {
    return keywords.find(str) != keywords.end();
}

// Function to check if a character is a punctuation
bool isPunctuation(char ch) {
    return punctuation.find(ch) != punctuation.end();
}

// Function to check if a string is an operator
bool isOperator(const string& str) {
    return operators.find(str) != operators.end();
}

// Function to check if a string is a valid identifier
bool isValidIdentifier(const string& str) {
    if (!isalpha(str[0]) && str[0] != '_') return false;
    for (char ch : str) {
        if (!isalnum(ch) && ch != '_') return false;
    }
    return true;
}

// Function to add an identifier to the symbol table
void addToSymbolTable(const string& identifier) {
    for (const string& entry : symbolTable) {
        if (entry == identifier) return; // Avoid duplicates
    }
    symbolTable.push_back(identifier);
}

// Function to tokenize the input
void tokenize(const string& line, int lineNumber) {
    string token;
    for (size_t i = 0; i < line.length(); i++) {
        char ch = line[i];

        // Skip whitespace
        if (isspace(ch)) continue;

        // Check for comments
        if (ch == '/' && i + 1 < line.length()) {
            if (line[i + 1] == '/') break; // Single-line comment
            if (line[i + 1] == '*') { // Multi-line comment
                i += 2;
                while (i + 1 < line.length() && !(line[i] == '*' && line[i + 1] == '/')) i++;
                i += 2;
                continue;
            }
        }

        // Check for punctuation
        if (isPunctuation(ch)) {
            cout << "Punctuation: " << ch << endl;
            continue;
        }

        // Check for operators
        if (i + 1 < line.length() && isOperator(string(1, ch) + string(1, line[i + 1]))) {
            cout << "Operator: " << ch << line[i + 1] << endl;
            i++;
            continue;
        }
        if (isOperator(string(1, ch))) {
            cout << "Operator: " << ch << endl;
            continue;
        }

        // Check for identifiers or keywords
        if (isalpha(ch) || ch == '_') {
            token = ch;
            while (i + 1 < line.length() && (isalnum(line[i + 1]) || line[i + 1] == '_')) {
                token += line[++i];
            }
            if (isKeyword(token)) {
                cout << "Keyword: " << token << endl;
            } else if (isValidIdentifier(token)) {
                cout << "Identifier: " << token << endl;
                addToSymbolTable(token);
            } else {
                cout << "Line " << lineNumber << ": " << token << " invalid lexeme" << endl;
            }
            continue;
        }

        // Check for constants (integers or characters)
        if (isdigit(ch) || ch == '\'') {
            token = ch;
            if (ch == '\'') { // Character constant
                if (i + 2 < line.length() && line[i + 2] == '\'') {
                    token += line[++i];
                    token += line[++i];
                    cout << "Constant: " << token << endl;
                } else {
                    cout << "Line " << lineNumber << ": " << token << " invalid lexeme" << endl;
                }
            } else { // Integer constant
                while (i + 1 < line.length() && isdigit(line[i + 1])) {
                    token += line[++i];
                }
                cout << "Constant: " << token << endl;
            }
            continue;
        }

        // Invalid character
        cout << "Line " << lineNumber << ": " << ch << " invalid lexeme" << endl;
    }
}

int main() {
    string fileName;
    cout << "Enter the input file name (e.g., input.c++): ";
    cin >> fileName;

    ifstream inputFile(fileName);
    if (!inputFile) {
        cerr << "Error: Unable to open input file '" << fileName << "'. Please check the file name and path." << endl;
        return 1;
    }

    string line;
    int lineNumber = 1;
    while (getline(inputFile, line)) {
        tokenize(line, lineNumber++);
    }

    // Print symbol table
    cout << "\nSYMBOL TABLE ENTRIES:" << endl;
    for (size_t i = 0; i < symbolTable.size(); i++) {
        cout << i + 1 << ") " << symbolTable[i] << endl;
    }

    inputFile.close();
    return 0;
}