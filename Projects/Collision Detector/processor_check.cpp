// Copyright 2017 Jeff Carruthers/EC327 jbc@bu.edu
// this program takes about 1 second to run on the server
// used for offline calibration of your code
#include <iostream>
#include <cstdint>
int main() {

  int64_t i;
  int64_t limit = 1'145'000'000UL;
  std::cout << limit << '\n';
  float x = 0.1; 
  for (i = 0; i < limit; i++)
    x += 1.0;
  
  std::cout << x << '\n';

}

