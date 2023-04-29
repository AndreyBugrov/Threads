#include "data_generator.h"
void BaseDataGenerator::operator()(double** matrix, const int n) {
}
void RandDataGenerator::operator()(double** matrix, const int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 1; j <= n; j++) {
      matrix[i][j] = (end_ - begin_) * (engine() % 2) + begin_;
    }
  }
}

void FileDataGenerator::operator()(double** matrix, const int n) {
  using std::string;
  string str;
  for (int i = 0; i < k_CORNER_POINTS; i++) {
    std::getline(is, str, '\n');
    string substr;
    string number;
    /*do {
      
    } while ()*/
  }
}

void ZeroDataGenerator::operator()(double** matrix, const int n) {
  for (int i = 1; i < n; i++) {
    for (int j = 1; j <= n; j++) {
      matrix[i][j] = 0;
    }
  }
}