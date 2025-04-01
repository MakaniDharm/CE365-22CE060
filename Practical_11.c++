#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

struct Quad {
    string op;
    string arg1;
    string arg2;
    string result;
};

vector<Quad> quadTable;
int tempCount = 1;
string input;
size_t pos = 0;

string newTemp() {
    return "t" + to_string(tempCount++);
}

char peek() {
    return (pos < input.length()) ? input[pos] : '\0';
}

char next() {
    return (pos < input.length()) ? input[pos++] : '\0';
}

void skipWhitespace() {
    while (isspace(peek())) next();
}

string parseF();
string parseT();
string parseE();

string parseF() {
    skipWhitespace();
    char c = peek();
    if (isdigit(c)) {
        string num;
        while (isdigit(peek())) num += next();
        return num;
    }
    if (c == '(') {
        next();
        string result = parseE();
        if (next() != ')') throw runtime_error("Missing ')'");
        return result;
    }
    throw runtime_error("Invalid F");
}

string parseT() {
    string left = parseF();
    skipWhitespace();
    char c = peek();
    while (c == '*' || c == '/' || c == '^') {
        next();
        string right = parseF();
        string temp = newTemp();
        quadTable.push_back({string(1, c), left, right, temp});
        left = temp;
        skipWhitespace();
        c = peek();
    }
    return left;
}

string parseE() {
    string left = parseT();
    skipWhitespace();
    char c = peek();
    while (c == '+' || c == '-') {
        next();
        string right = parseT();
        string temp = newTemp();
        quadTable.push_back({string(1, c), left, right, temp});
        left = temp;
        skipWhitespace();
        c = peek();
    }
    return left;
}

void generateQuadruples(string expr) {
    input = expr;
    pos = 0;
    quadTable.clear();
    tempCount = 1;
    parseE();
    if (pos < input.length()) throw runtime_error("Invalid expression");
}

int main() {
    string expr;
    cout << "Enter arithmetic expression: ";
    getline(cin, expr);
    try {
        generateQuadruples(expr);
        cout << "Operator\tOperand 1\tOperand 2\tResult\n";
        for (const auto& q : quadTable) {
            cout << q.op << "\t\t" << q.arg1 << "\t\t" << q.arg2 << "\t\t" << q.result << "\n";
        }
    } catch (const exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
    return 0;
}