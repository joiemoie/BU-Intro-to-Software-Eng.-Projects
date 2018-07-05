// Copyright 2017 Joseph Liba joiemoie@bu.edu

#include<iostream>
#include<fstream>
#include<vector>

using std::vector;
using std::string;
using std::istream;
using std::ostream;

const int asterisk = 42, period = 46;

void RotateMatrixBy90(vector< std::string > * board) {
  int size = (*board).size();
  for (int i=0; i<size/2; i++)
    for (int j=0; j<(size+1)/2; j++){
      char temp = (*board)[i][j];
      (*board)[i][j] = (*board)[size-1-j][i];
      (*board)[size-1-j][i] = (*board)[size-1-i][size-1-j];
      (*board)[size-1-i][size-1-j] = (*board)[j][size-1-i];
      (*board)[j][size-1-i] = temp;
    }
}

void FlipMatrix(vector< std::string > * board) {
  int size = (*board).size();
  for (int i=0; i<size/2; i++) {
    for (int j=0; j<size; j++) {
      char temp = (*board)[i][j];
      (*board)[i][j] = (*board)[size-1-i][j];
      (*board)[size-1-i][j] = temp;
    }
  }
}

bool BoardsAreEqual(vector< std::string > * board1, vector< std::string > * board2){
  if ((*board1).size() != (*board2).size()) return false;
  vector < int > offset = {0,0}, firstPoint = {0,0}, secondPoint = {0,0};
  bool firstSet = false, secondSet = false;
  bool matchFound = false;
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < 4; i++) {
      bool rotateMatchFound = true;
      for (int k = 0; k < (*board1).size(); k++)
        for (int l = 0; l < (*board1).size(); l++) {
          if (!firstSet && static_cast<int>((*board1)[k][l])==asterisk)
            firstPoint = {k,l};
          if (!secondSet && static_cast<int>((*board2)[k][l])==asterisk)
            secondPoint = {k,l};
          if (!(firstSet || secondSet))
              offset = {firstPoint[0] - secondPoint[0], firstPoint[1] - secondPoint[1]};
        }
      for (int k = 0; k < (*board1).size(); k++)
        for (int l = 0; l < (*board1).size(); l++) {
          if (k+offset[0]<0 || l+offset[1]<0)
            { if (static_cast<int>((*board1)[k][l])==asterisk) rotateMatchFound = false; }
          else if (static_cast<int>((*board1)[k][l])!=static_cast<int>((*board2)[k+offset[0]][l+offset[1]])) rotateMatchFound = false;
        }
      if (rotateMatchFound) matchFound = true;
      RotateMatrixBy90(board1);
    }
    FlipMatrix(board1);
  }
  return matchFound;
}

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
        if (static_cast<int>(c) != asterisk && static_cast<int>(c) != period) return 1;
        if (static_cast<int>(c) == asterisk) boardEmpty = false;
      }
      if (str.length() != size) return 2;
      board.push_back(str);
    }
    if (isBoard) {
      if (board.size() != size)  return 2;
      if (boardEmpty) return 3;
      bool boardExists = false;
      for (int i = 0; i < boards.size(); i++) {
        if (BoardsAreEqual(&boards[i], &board)) boardExists = true;;
      }
      if (!boardExists) {
        boards.push_back(board);
      }
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