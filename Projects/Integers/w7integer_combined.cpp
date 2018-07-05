#include <iostream>
#include <string>
#include "w7integer.h"

using std::string;
using std::cout;
using std::cin;

void basics() {

  Integer i("1234");
  Integer j(-321);

  Integer k;

}

void addition() {
  Integer i("1234");
  Integer j(2847);

  i + j;
  j + j + j + j + j + j;
  i + 14;
  14 + j;

}

void multiply() {
    Integer i(12);
  cout << i * 2 << ' ' << 3 * i << ' ' <<  i*i + 4 << '\n';

  string val1[] = {"6291021234", "100", "-100", "1099", "-200", "-400", "-301", "12", "0", "-12"};
  string val2[] = {"6291021234", "99", "100", "911", "-100", "300", "-301", "0", "122", "0"};

  for (int i = 0; i < 10; i++) {
    Integer ii(val1[i]);
    Integer jj(val2[i]);

    cout << ii*jj << '\n';
  }
  
}

void output() {

  Integer i("1234");
  Integer j(123);
  Integer k("-1230001");
  Integer z;

  std::cout << i << ' ' << j << ' ' << k << ' ' << z << '\n';

}


void compare(string i, string j) {
  Integer ii(i);
  Integer jj(j);

  std::cout << ( (ii > jj) ? (i + ">" + j) : ("not " + i + ">" + j)) << '\n';
  std::cout << ( (ii < jj) ? (i + "<" + j) : ("not " + i + "<" + j)) << '\n';

  std::cout << ( (ii == jj) ? (i + "==" + j) : ("not " + i + "==" + j)) << '\n';
  std::cout << ( (ii != jj) ? (i + "!=" + j) : ("not " + i + "!=" + j)) << '\n';

  std::cout << ( (ii >= jj) ? (i + ">=" + j) : ("not " + i + ">=" + j)) << '\n';
  std::cout << ( (ii <= jj) ? (i + "<=" + j) : ("not " + i + "<=" + j)) << '\n';

}

void testbool(string s)  {
  std::cout << ( Integer(s) ? s + " is true" : s + " is false")  << '\n';
}

void logic() {
  string val1[] = {"6291021234", "100", "-100", "1099", "-200", "-400", "-301"};
  string val2[] = {"6291021234", "99", "100", "911", "-100", "300", "-301"};

  for (int i = 0; i < 7; i++)
    compare(val1[i], val2[i]);

  string s[] = {"2", "0", "-3", "100", "-0"};

  for (int i = 0; i < 5; i++)
    testbool(s[i]);

}


void subtract() {
  Integer i("1234");
  Integer j(2847);

  cout << i - j << ' ' << j - i << '\n';

  i = 10000;
  j = 9912;
  cout << i - j << ' ' << j - i << '\n';

  i = 0;
  j = 1990;
  cout << i - j << ' ' << j - i << '\n';

  i = -689;
  j = 1232;
  cout << i - j << ' ' << j - i << '\n';

  i = -689;
  j = -1232;
  cout << i - j << ' ' << j - i << '\n';

  i = 689;
  j = -1232;
  cout << i - j << ' ' << j - i << '\n';

  i = 689;
  j = 1232;
  cout << i - j << ' ' << j - i << '\n';
}


void limits() {
  Integer i(1),j;
  cout << i << '\n';

  while (cin >> j){
    i = i * j;
    cout << i << '\n';
  }
  
}

void input() {
  Integer i;
  Integer total;

  while (cin >> i) {
    cout << i << '\n';
    total = total + i;
  }

  cout << total << '\n';
}

void increment() {

  Integer i("1234");
  Integer j, k;

  j = i++;
  k = ++i;
  cout << i << ' ' << j << ' ' << k << '\n';

  i = string("1000000");
  j = i--;
  k = --i;
  cout << i << ' ' << j << ' ' << k << '\n';

  i = string("-1000000");
  j = i--;
  k = --i;
  cout << i << ' ' << j << ' ' << k << '\n';

  i = string("9999999999");
  j = i++;
  k = ++i;
  cout << i << ' ' << j << ' ' << k << '\n';

}

void compound() {

  Integer i(999);
  Integer j(1002);
  Integer k(100000);

  i += Integer(2);
  j *= Integer(3);
  k -= Integer("122");

  cout << i << ' ' << j << ' ' << k << '\n';

  i /= Integer(2);
  k = 18;
  k /= Integer(6);
  j %= Integer(6);


  cout << i << ' ' << j << ' ' << k << '\n';

}

void divide() {

  Integer i("1234");
  Integer j("12");
  cout << i << '/' << j << '=' << (i / j) << 'r' << i % j << '=' << (i / j)*j + i % j << '\n';

  i = -1234;
  j = -12;
  cout << i << '/' << j << '=' << (i / j) << 'r' << i % j << '=' << (i / j)*j + i % j << '\n';

  i = -1234;
  j = 12;
  cout << i << '/' << j << '=' << (i / j) << 'r' << i % j << '=' << (i / j)*j + i % j << '\n';

  i = 1234;
  j = -12;

  cout << i << '/' << j << '=' << (i / j) << 'r' << i % j << '=' << (i / j)*j + i % j << '\n';

  i = 200;
  j = 25;
  cout << i << '/' << j << '=' << (i / j) << 'r' << i % j << '=' << (i / j)*j + i % j << '\n';

  i = 25;
  j = 200;
  cout << i << '/' << j << '=' << (i / j) << 'r' << i % j << '=' << (i / j)*j + i % j << '\n';

  Integer k(12);
  cout << k / 2 << ' ' << 30 / k << ' ' <<  30 % k << ' ' << k % 7 << '\n';

}

void zero() {

  Integer z1("0000");
  Integer z2("-0");
  Integer large("1000000000000000");
  std::cout << "zero: " << z1 << '\n';
  std::cout << "zero: " << z2 << '\n';

  std::cout << "zero: " << Integer(100) + Integer(-100) << '\n';
  std::cout << "zero: " << Integer(100) - 100 << '\n';
  std::cout << "zero: " << 1000  - Integer(1000) << '\n';
  std::cout << "zero: " << large - large << '\n';
  z1 = large - large;
  std::cout << "zero: " << z1 << '\n';

  std::cout << "zero: " << (Integer("1000000000000000") - Integer("1000000000000000")) << '\n';

  z2 = Integer(300) - Integer(400) + Integer(100);
  std::cout << "zero: " << z2 << '\n';

}
int main() {
  basics();
  addition();
  //output();  
  //input();  
  //subtract();
  increment();  
  logic();
  zero();
  multiply();
  divide();
  compound();

  return 0;
}