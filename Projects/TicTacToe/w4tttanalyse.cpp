// Copyright 2017 Joseph Liba joiemoie@bu.edu

#include<iostream>
#include<fstream>

int main() {
  std::ifstream myReadFile;
  std::fstream tttstatus;
  tttstatus.open("tttstatus.txt", std::fstream::out);
  myReadFile.open("tictactoeboards.txt");
  char board[100];
  board[0] = 'a';
  board[1] = 'b';
  if (myReadFile.is_open()) {
    std::string str;
    while (std::getline(myReadFile, str)) {
      char result = '.';
      int winners = 0;
      int numX = 0;
      int numO = 0;
      for (int i = 0; i < str.length(); i++) {
        if (str[i] == 'x') numX++;
        if (str[i] == 'o') numO++;
      }
      if (str[0] == str[4] && str[0] == str[8] && str[0] != '#') {
        if (result != str[0]) winners++;
        result = str[0];
      }
      if (str[2] == str[4] && str[2] == str[6] && str[2] != '#') {
        if (result != str[2]) winners++;
        result = str[2];
      }
      for (int i = 0; i < 3; i++) {
        if (str[i] == str[3 + i] && str[i] == str[6 + i] && str[i] != '#') {
          if (result != str[i]) winners++;
          result = str[i];
        }
        if (str[i*3] == str[1+ i*3] && str[i*3] == str[2+ i*3]) {
          if (str[i*3] != '#') {
            if (result != str[3*i]) winners++;
            result = str[3*i];
          }
        }
      }
      if (winners == 0) {
        if (numX + numO == 9) result = 't';
        else
          result = 'c';
      }
      if (winners > 1) result = 'i';
      if (numX - numO > 1 || numX - numO < 0) result = 'i';
      if (result == 'x' && numX - numO != 1) result = 'i';
      if (result == 'o' && numO - numX != 0) result = 'i';
      tttstatus << str << " "<< result <<  std::endl;
    }
  }
  myReadFile.close();
  tttstatus.close();
  return 0;
}
