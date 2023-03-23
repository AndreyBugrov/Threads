#include "data_generator.h"
void RandDataGenerator::operator()(double** matrix, const int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i][j] = (end_ - begin_) * (engine() % 2) + begin_;
    }
  }
}

void FileDataGenerator::operator()(double** matrix, const int n) {
  string str;
  for (int i = 0; i < NumDirichlet::k_CORNER_POINTS; i++) {
    std::getline(is, str, '\n');
    string substr;
    string number;
    /*do {
      
    } while ()*/
  }
}

void ZeroDataGenerator::operator()(double** matrix, const int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i][j] = 0;
    }
  }
}