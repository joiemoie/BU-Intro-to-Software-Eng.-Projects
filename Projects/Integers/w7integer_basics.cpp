#include <iostream>
#include "w7integer.h"

using std::cout;
int main() {

  Integer i("2002");
  Integer j("1003");
  std::cout << "*" << ((i+j+1));
  Integer k;
  std::cin >> k;
  return 0;

}