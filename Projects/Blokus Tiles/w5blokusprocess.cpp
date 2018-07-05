// Copyright 2017 Joseph Liba joiemoie@bu.edu

#include<iostream>
#include<fstream>
#include<vector>

using std::vector;
using std::string;
using std::istream;
using std::ostream;

int ReadBlokusTiles(int size, istream* input, ostream* output) {
  vector< vector< std::string > > boards;
  vector< std::string > board;
  std::string str;
  while (!(*input).eof()) {
    bool boardEmpty = true;
    bool isBoard = false;
    while (std::getline((*input), str) && str != "") {
      isBoard = true;
      for (char& c : str) {
        if (static_cast<int>(c) != 42 && static_cast<int>(c) != 46) return 1;
        if (static_cast<int>(c) == 42) boardEmpty = false;
      }
      if (str.length() != size) return 2;
      board.push_back(str);
    }
    if (isBoard) {
      if (board.size() != size)  return 2;
      if (boardEmpty) return 3;
      boards.push_back(board);
      board.clear();
    }
  }
  for (int j = 0; j < boards.size(); j++) {
    for (int i = 0; i < boards[j].size(); i++) {
      (*output) << boards[j][i] << '\n';
    }
    (*output) << '\n';
  }
  return 0;
}

int main(int argc, const char* argv[]) {
  istream* input = &std::cin;
  ostream* output = &std::cout;
  std::ifstream inputFile;
  std::ofstream outputFile;
  int size = 5;
  if (argc > 1) size = atoi(argv[1]);
  if (argc > 2) {
    inputFile.open(argv[2]);
    input = &inputFile;
  }
  if (argc > 3) {
    outputFile.open(argv[3], std::fstream::out);
    output = &outputFile;
  }
  int result = ReadBlokusTiles(size, input, output);
  if (argc > 2) inputFile.close();
  if (argc > 3) outputFile.close();
  return result;
}
