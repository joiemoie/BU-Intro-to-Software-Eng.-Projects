// Copyright 2017 Joseph Liba joiemoie@bu.edu

#include<iostream>
#include<fstream>
#include<vector>

using std::vector;
using std::string;
using std::istream;
using std::ostream;

const int asterisk = 42, period = 46;

vector< vector< int > > * BoardCoordinates(vector < vector < int > > * board) {
  vector< vector< int > > coordinateBoard;
  for (int i = 0; i < (*board).size(); i++) {
    for (int j = 0; j < (*board)[i].length(); j++) {
      if (static_cast<int>((*board)[i][j]) == asterisk)) boards.pushBack({j,i});
    }
  }
  return &coordinateBoard;
}

void RotatePointsBy90(vector< vector < int > > * board) {
  float centerPoint = (float)((*board).size()-1) / 2
  for (int i = 0; i < (*board).size(); i++) {
    float xDist = (*board)[i][0] - center, yDist = (*board)[i][1] - center;
    if (xDist < 0 ^ yDist < 0) (*board)[i][0] = static_cast<int>(center - xDist); 
    if (xDist > 0 ^ yDist < 0) (*board)[i][1] = static_cast<int>(center - yDist);
  }
}

void FlipMatrix(vector< vector < int > > * board) {
  float centerPoint = (float)((*board).size()-1) / 2
  for (int i = 0; i < (*board).size(); i++) {
    float xDist = (*board)[i][0] - center, yDist = (*board)[i][1] - center;
    (*board)[i][0] = static_cast<int>(center - xDist); 
  }
}

bool CoordinateBoardsAreEqual(vector< vector< int > > * board1, vector< vector< int > > * board2){
  if ((*board1).size() != (*board2).size) return false;
  vector < int > offset = {0,0};
  bool offsetDetermined = false;
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < 4; i++) {
      bool matchFound = true;
      for (int i = 0; i < (*board1).size(); i++){
        if (offsetDetermined)
          if (offset[0] != (*board1)[i][0] - (*board2)[i][0]) matchFound = false;
          if (offset[1] != (*board1)[i][1] - (*board2)[i][1]) matchFound = false;
        else {
          offset[0] = (*board1)[i][0] - (*board2)[i][0]);
          offset[1] = (*board1)[i][1] - (*board2)[i][1]);
          offsetDetermined = true;
        }
      }
      RotateMatrixBy90(*board1);
    FlipMatrix(*board1);
  }
  return matchFound;
}

int ReadBlokusTiles(int size, istream* input, ostream* output) {
  vector< vector< std::string > > boards;
  vector< vector< vector< int > > > > coordinateBoards;
  vector< vector< std::string > > normalizedBoards;
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
      boardCoordinates* = BoardCoordinates(board);
      bool boardExists = false;
      for (int i = 0; i < coordinateBoards.size(); i++) {
        if (boards[i].size() == board.size())
          if (CoordinateBoardsAreEqual(&coordinateBoards[i], boardCoordinates)) boardExists;
      }
      if (!boardExists) {
        boards.push_back(board);
        coordinateBoards.push_back(BoardCoordinates(board));
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