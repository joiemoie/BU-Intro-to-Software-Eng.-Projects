// Copyright 2017 Joseph Liba joiemoie@bu.edu

#ifndef _MEDIA_SF_EC327_SPRING2017_PROJECTS_INTEGERS_W7INTEGER_H_
#define _MEDIA_SF_EC327_SPRING2017_PROJECTS_INTEGERS_W7INTEGER_H_
#include <iostream>
#include <string>

using std::string;

class Integer {
 private:
  bool neg = false;
  string digits;

  string padZeroes(string str, int numZeroes) {
    string zeroes = "";
    for (int i = 0; i < numZeroes; i++) {
      zeroes.append("0");
    }
    return zeroes.append(str);
  }


  void MatchZeroes(string& string1, string& string2) {
      if (string1.length() < string2.length()) {
        string1 = padZeroes(string1, string2.length()-string1.length()+1);
        string2 = padZeroes(string2, 1);
      } else {
        string2 = padZeroes(string2, string1.length() - string2.length()+1);
        string1 = padZeroes(string1, 1);
      }
  }

  string positiveAdder(string string1, string string2) {
    string result;
    int carry = 0;
    for (int i = string1.length()-1; i >= 0; i--) {
      int sum = string1[i] - '0' + string2[i] - '0' + carry;
      carry = sum / 10;
      sum %= 10;
      result.insert(0, std::to_string(sum));
    }
    return result;
  }

  string complement(string val) {
    for (char&c : val) {
      c = '0' + '9' - c;
    }
     string one = "";
    for (int i = 0; i < val.length()-1; i++) {
      one.append("0");
    }
    one.append("1");
    return positiveAdder(val, one);
  }

  string fullAdder(string string1, bool neg1, string string2, bool neg2) {
    MatchZeroes(string1, string2);
    if (neg1) string1 = complement(string1);
    if (neg2) string2 = complement(string2);
    return positiveAdder(string1, string2);
  }
  Integer IntAbs();

 public:
  Integer(int val) {
    if (val > 0) neg = false;
    else {
      neg = true;
      val *= -1;
    }
    digits.append(std::to_string(val));
  }
  Integer() { neg = false; digits = "0"; }
  Integer(string str) {
    string newDigits;
    if (str[0] == '-') {
      newDigits.append(str.substr(1));
      neg = true;
    } else {
      newDigits = str;
    }
    int i = 0;
    while (newDigits[i] == '0') i++;
    digits.append(newDigits.substr(i));
    if (digits == "") { neg = false; digits.append("0"); }
  }
  explicit operator bool() const { return digits != "0"; }
  Integer operator+(Integer int2);
  Integer operator-(Integer int2);
  Integer operator-();
  Integer operator*(Integer int2);
  Integer operator/(Integer int2);
  Integer operator%(Integer int2);
  bool operator==(Integer int2);
  bool operator!=(Integer int2);
  bool operator>(Integer int2);
  bool operator<(Integer int2);
  bool operator>=(Integer int2);
  bool operator<=(Integer int2);
  Integer& operator+=(Integer int2);
  Integer& operator-=(Integer int2);
  Integer& operator*=(Integer int2);
  Integer& operator/=(Integer int2);
  Integer& operator%=(Integer int2);
  Integer operator++(const int);
  Integer operator--(const int);
  Integer operator++();
  Integer operator--();
  friend std::ostream& operator<<(std::ostream& os, Integer int1);
  friend std::istream& operator>>(std::istream& is, Integer& int1);
};

Integer Integer::operator+(Integer int2) {
  string digits = this->digits;
  string result = this->fullAdder(digits, this->neg, int2.digits, int2.neg);
  if (result[0] == '9') {
    result = this->complement(result);
    result.insert(0, "-");
  }
  return Integer(result);
}

Integer operator+(int int1, Integer int2) {
  return Integer(int1)+int2;
}

Integer Integer::operator-(Integer int2) {
  int2.neg = !int2.neg;
  return this->operator+(int2);
}

Integer operator-(int int1, Integer int2) {
  return Integer(int1)-int2;
}

Integer Integer::operator-() { return Integer(0)-*this; }

Integer Integer::operator*(Integer int2) {
  string result;
  string digits1 = this->digits;
  string digits2 = int2.digits;
  this->MatchZeroes(digits1, digits2);
  for (int i = 0; i < digits1.length() + digits2.length(); i++) {
    result.append("0");
  }
  for (int i = digits1.length()-1; i >= 0 ; i--) {
    int carry = 0;
    for (int j = digits2.length()-1; j >= 0; j--) {
      int num1 = (digits1[j] - '0');
      int num2 =  (digits2[i] - '0');
      int sum = num1 * num2 + result[i+j+1] + carry - '0';
      carry = sum / 10;
      result[i+j+1] = '0' + sum%10;
    }
  }
  bool neg = this->neg ^ int2.neg;
  if (neg) result.insert(0, "-");
  return result;
}

Integer operator*(int int1, Integer int2) {
  return Integer(int1)*int2;
}

bool Integer::operator==(Integer int2) { return (*this-int2).digits == "0"; }
bool Integer::operator!=(Integer int2) { return (*this-int2).digits != "0"; }
bool Integer::operator>(Integer int2) {
  return !(*this-int2).neg && *this != int2;
}
bool Integer::operator<(Integer int2) { return (*this-int2).neg; }
bool Integer::operator>=(Integer int2) { return !(*this-int2).neg; }
bool Integer::operator<=(Integer int2) {
  return (*this-int2).neg || *this == int2;
}

bool operator==(int int1, Integer int2) { return Integer(int1) == int2; }
bool operator!=(int int1, Integer int2) { return Integer(int1) != int2; }
bool operator>(int int1, Integer int2) { return Integer(int1) > int2; }
bool operator<(int int1, Integer int2) { return Integer(int1) < int2; }
bool operator>=(int int1, Integer int2) { return Integer(int1) >= int2; }
bool operator<=(int int1, Integer int2) { return Integer(int1) <= int2; }

Integer Integer::IntAbs() { return (this->neg)?this->operator*(-1):*this; }

Integer Integer::operator/(Integer int2) {
  string result;
  string digits1 = this->digits;
  string digits2 = int2.digits;
  for (int i = 0; i < digits1.length(); i++) {
    result.append("0");
  }
  int marker = 0;
  Integer subInt = Integer(0);
  while (marker < digits1.length()) {
    subInt = subInt * 10 + Integer(digits1.substr(marker, 1));
    int i = 1;
    while (i < 10 && (int2.IntAbs()*i <= subInt)) i++;
    i--;
    result[marker] = i + '0';
    subInt = subInt - int2.IntAbs() * i;
    marker++;
  }
  bool neg = this->neg ^ int2.neg;
  if (neg) {
    Integer res(result);
    res++;
    result = res.digits;
    result = "-" + result;
  }
  return Integer(result);
}

Integer operator/(int int1, Integer int2) {
  return Integer(int1)/int2;
}

Integer Integer::operator%(Integer int2) {
  bool neg = this->neg;
  return (this->IntAbs()-((*this/int2)*int2).IntAbs()) * (neg?-1:1);
}

Integer operator%(int int1, Integer int2) {
  return Integer(int1)%int2;
}

Integer& Integer::operator+=(Integer int2) {
  *this = (*this) + int2; return *this;
}
Integer& Integer::operator-=(Integer int2) {
  *this = (*this) - int2; return *this;
}
Integer& Integer::operator*=(Integer int2) {
  *this = (*this) * int2; return *this;
}
Integer& Integer::operator/=(Integer int2) {
  *this = (*this) / int2; return *this;
}
Integer& Integer::operator%=(Integer int2) {
  *this = (*this) % int2; return *this;
}

Integer& operator+=(int int1, Integer int2) { Integer(int1) += int2; }
Integer& operator-=(int int1, Integer int2) { Integer(int1) -= int2; }
Integer& operator*=(int int1, Integer int2) { Integer(int1) *= int2; }
Integer& operator/=(int int1, Integer int2) { Integer(int1) /= int2; }
Integer& operator%=(int int1, Integer int2) { Integer(int1) %= int2; }

Integer Integer::operator++(const int) {
  (*this) += 1;
  return (*this) - 1;
}
Integer Integer::operator--(const int) {
  (*this) -= 1;
  return (*this) + 1;
}
Integer Integer::operator++() {
  (*this)+=1;
  return *this;
}
Integer Integer::operator--() { (*this) -= 1; return *this; }

std::ostream& operator<<(std::ostream& os, Integer int1) {
  os << ((int1.neg)?"-":"") << int1.digits;
  return os;
}
std::istream& operator>>(std::istream& is, Integer& int1) {
  string input;
  is >> input;
  int1 = Integer(input);
  return is;
}

#endif  // _MEDIA_SF_EC327_SPRING2017_PROJECTS_INTEGERS_W7INTEGER_H_
