#include<iostream>
#include<string>

using namespace std;

class Parser {
  string input;
  size_t pos;

  bool S(){
    if(pos<input.length() && input[pos] == '(' ){
      pos++;
      L();
      if(pos<input.length() && input[pos] == ')'){
        pos++;
        return true;
      }
      return false;
    }
    else if(pos<input.length() && input[pos] == 'a'){
      pos++;
      return true;
    }
  }
  bool L(){
    if (S()){
      return Ldash();
    }
    return false;
  }

  bool Ldash(){
    if(pos<input.length() && input[pos] == ','){
      pos++;
      if(S()){
        return Ldash();
      }
      return false;
    }
    return true;
  }
public:
  Parser (const string& str): input(str) ,pos(0) {}

  bool parse() {
    return S() && pos == input.length();
  }
};
int main(){
  string str;
  cout << "Enter a String:";
  cin >> str;
  
  Parser parser(str);
  if (parser.parse()){
    cout << "String is valid" << endl;
  } 
  else {
      cout << "String is not valid" << endl;
    }
    return 0;
  
}