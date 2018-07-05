// Copyright 2017 Joseph Liba joiemoie@bu.edu

#include <iostream>

using std::cout;
using std::endl;

int main() {
  for (int i = 2 ; i <=888; i++) {
    int sum = 0;
    bool firstNum = false;
    cout << i << ':';
    for (int j = 1; j < i; j++) {
      if (i % j == 0) {
        sum += j;
        if (firstNum) {
          cout << '+' << j;
        } else {
          firstNum = true;
          cout << j;
        }
      }
    }
    cout << '=' << sum << endl;
  }
  return 0;
}


