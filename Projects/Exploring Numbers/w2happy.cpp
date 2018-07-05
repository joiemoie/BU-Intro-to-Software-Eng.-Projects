// Copyright 2017 Joseph Liba joiemoie@bu.edu

#include <iostream>

using std::cout;
using std::endl;

int happy(int n) {
  if (n == 4) return 4;
  if (n == 1) return 1;
  int sum = 0;
  int digit;
  do {
      digit = n%10;
      sum+=digit * digit;
      n/=10;
  } while (n != 0);
  return happy(sum);
}

int main() {
  int i = 0, j = 1;
  while (i <= 888) {
    bool found = false;
    int result = happy(j);
    if (result == 1) {
      cout << j << " ";
      i++;
    }
    j++;
  }
  return 0;
}


