// Copyright 2017 Joseph Liba joiemoie@bu.edu

#include<iostream>
#include<fstream>
#include<vector>

using std::vector;
using std::string;
using std::istream;
using std::ostream;

const int asterisk = 42, period = 46;

void TraverseTiles(vector<int> point, vector<vector<int>>* pointsTraversed, vector<std::string>* board);

void TraverseTilesHelper(vector<int> newPoint, vector<vector<int>>* pointsTraversed, vector<std::string>* board) {
  int i = 0;
  while (i < (*pointsTraversed).size() && (*pointsTraversed)[i] != newPoint) i++;
  if (i == (*pointsTraversed).size())
    if (newPoint[0] >= 0 && newPoint[1] >= 0 && newPoint[0] < (*board).size() && newPoint[1] < (*board).size())
      TraverseTiles(newPoint, pointsTraversed, board);
}

void TraverseTiles(vector<int> point, vector<vector<int>>* pointsTraversed, vector<std::string>* board) {
  if (static_cast<int>((*board)[point[0]][point[1]]) == asterisk) {
    (*pointsTraversed).push_back(point);
    vector < int > newPoint = {point[0]-1, point[1]};
    TraverseTilesHelper(newPoint, pointsTraversed, board);
    newPoint = {point[0]+1, point[1]};
    TraverseTilesHelper(newPoint, pointsTraversed, board);
    newPoint = {point[0], point[1]-1};
    TraverseTilesHelper(newPoint, pointsTraversed, board);
    newPoint = {point[0], point[1]+1};
    TraverseTilesHelper(newPoint, pointsTraversed, board);
  }
}

bool HasHoles(vector< std::string > * board) {
  bool pointsSet = false;
  vector<vector<int>> pointsTraversed;
  for (int i = 0; i < (*board).size(); i++)
    for (int j = 0; j < (*board).size(); j++)
      if (static_cast<int>((*board)[i][j]) == asterisk) {
        if (!pointsSet) {
          TraverseTiles({i, j}, &pointsTraversed, board);
          pointsSet = true;
        } else {
          int k = 0;
          vector<int> newPoint = {i, j};
          while (k < pointsTraversed.size() && (pointsTraversed[k] != newPoint)) k++;
          if (k == pointsTraversed.size()) return true;
        }
      }

  return false;
}

void RotateMatrixBy90(vector< std::string > * board) {
  int size = (*board).size();
  for (int i = 0; i < size/2; i++) {
    for (int j = 0; j < (size+1)/2; j++) {
      int temp = (*board)[i][j];
      (*board)[i][j] = (*board)[size-1-j][i];
      (*board)[size-1-j][i] = (*board)[size-1-i][size-1-j];
      (*board)[size-1-i][size-1-j] = (*board)[j][size-1-i];
      (*board)[j][size-1-i] = temp;
    }
  }
}

void FlipMatrix(vector< std::string > * board) {
  int size = (*board).size();
  for (int i = 0; i < (size+1)/2; i++) {
    for (int j = 0; j < size; j++) {
      int temp = (*board)[i][j];
      (*board)[i][j] = (*board)[size-1-i][j];
      (*board)[size-1-i][j] = temp;
    }
  }
}

bool BoardsAreEqual(vector< std::string > * board1, vector< std::string > * board2) {
  if ((*board1).size() != (*board2).size()) return false;
  int size = (*board2).size();
  bool matchFound = false;
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < 4; i++) {
      int minX = size, minY = size;
      int maxX = 0, maxY = 0;
      for (int q = 0; q < (*board2).size(); q++)
        for (int r = 0; r < (*board2).size(); r++) {
          if (static_cast<int>((*board2)[r][q]) == asterisk) {
            if (q < minX) minX = q;
            if (r < minY) minY = r;
            if (q > maxX) maxX = q;
            if (r > maxY) maxY = r;
          }
        }
      for (int k = -minY; k < size - maxY; k++) {
        for (int l = -minX; l < size - maxX; l++) {
          bool rotateMatchFound = true;
          for (int m = 0; m < (*board2).size(); m++) {
            for (int n = 0; n < (*board2).size(); n++)
              if (m-k >=0 && m-k < size && n-l >=0 && n-l <size) {
                if ((*board1)[m][n] != (*board2)[m-k][n-l]) rotateMatchFound = false;
              } else {
                  if ((*board1)[m][n] == '*') rotateMatchFound = false;
              }
          }
          if (rotateMatchFound) matchFound = true;
        }
      }
      RotateMatrixBy90(board2);
    }
    FlipMatrix(board2);
  }
  return matchFound;
}

int ReadBlokusTiles(int size, istream* input, ostream* output) {
  vector<vector<std::string>> boards;
  vector<std::string> board;
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
        if (BoardsAreEqual(&(boards[i]), &board)) boardExists = true;
      }
      if (!boardExists && !HasHoles(&board)) {
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
