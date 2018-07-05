// Copyright 2017 Joseph Liba joiemoie@bu.edu
// w3strings

#include <iostream>
#include <string>
using std::string;
using std::cout;
using std::cin;

bool is_palindrome(string num, bool & error) {
  if (static_cast<int>(num[0]) < 49 || static_cast<int>(num[0]) > 57) {
    error = true;
    return false;
  }
  for (int i = 0; i < num.length(); i++) {
    if (static_cast<int>(num[i]) < 48 || static_cast<int>(num[i]) > 57) {
      error = true;
      return false;
    }
  }
  for (int i = 0; i < num.length(); i++) {
    if (num[i] != num[num.length() - 1 - i]) {
      error = false;
      return false;
    }
  }
  error = false;
  return true;
}


bool is_word(string s) {
  if (s.length() == 0) return false;
  if (static_cast<int>(s[0]) >= 97 && static_cast<int>(s[0]) <= 122) {
    for (int i = 1; i < s.length(); i++) {
      if (static_cast<int>(s[i]) < 97 || static_cast<int>(s[i]) > 122) {
        return false;
      }
    }
  } else if (static_cast<int>(s[0]) >= 65 && static_cast<int>(s[0]) <= 90) {
    if (s.length() == 1) return true;
    if (static_cast<int>(s[1]) >= 65 && static_cast<int>(s[1]) <= 90) {
      for (int i = 2; i < s.length(); i++) {
        if (static_cast<int>(s[i]) < 65 || static_cast<int>(s[i]) > 90) {
          return false;
        }
      }
    } else if (static_cast<int>(s[1]) >= 97 && static_cast<int>(s[1]) <= 122) {
      for (int i = 2; i < s.length(); i++) {
        if (static_cast<int>(s[i]) < 97 || static_cast<int>(s[i]) > 122) {
          return false;
        }
      }
    } else { return false; }
  } else { return false; }
  return true;
}

// leave this line and everything below as is
int main() {
  cout << std::boolalpha;

  if (not is_word("test")) cout << "we1\n";
  if (not is_word("Test")) cout << "we2\n";
  if (not is_word("TEST")) cout << "we3\n";
  if (not is_word("thisisaword")) cout << "we4\n";

  if (is_word("123")) cout << "we5\n";
  if (is_word("")) cout << "we6\n";
  if (is_word("abc123abc")) cout << "we7\n";
  if (is_word("tEst")) cout << "we8\n";
  if (is_word("tEst")) cout << "we9\n";
  if (is_word("TESTer")) cout << "we10\n";

  bool error;

  if (not is_palindrome("12321", error) or error) cout << "pe1\n";
  if (not is_palindrome("9009", error) or error) cout  << "pe2\n";
  if (not is_palindrome("9", error) or error)  cout << "pe3\n";
  if (not is_palindrome("123456777654321", error) or error) cout << "pe4\n";

  if (is_palindrome("abcba", error) or not error) cout << "pe5\n";
  if (is_palindrome("12321 a", error) or not error) cout << "pe6\n";
  if (is_palindrome("0012100", error) or not error) cout << "pe7\n";

  if (is_palindrome("123", error) or error) cout << "pe8\n";
  if (is_palindrome("123211", error) or error) cout << "pe9\n";

  cout << "Test Other Words. Ctrl-D to exit.\n";

  string s;
  bool pal;
  while (cin >> s) {
    pal = is_palindrome(s, error);
    cout << s << " " << pal << " " << error << " " << is_word(s) << '\n';
  }

  return 0;
}
