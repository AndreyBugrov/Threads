#pragma once
#include <time.h>  // random gen

#include <fstream>  // file gen
#include <random>   // random gen
#include <string>   // getline - file gen

class BaseDataGenerator {
 public:
  virtual void operator()(double** matrix, const int n);
  BaseDataGenerator() = default;
  virtual ~BaseDataGenerator() = default;
  BaseDataGenerator(const BaseDataGenerator& gen) = delete;
  //  BaseDataGenerator(BaseDataGenerator&& gen) = delete;
};

class RandDataGenerator : public BaseDataGenerator {
 protected:
  double begin_;
  double end_;
  std::mt19937 engine;

 public:
  RandDataGenerator(double begin = 0, double end = 1)
      : begin_(begin), end_(end), engine(time(NULL)) {}
  virtual void operator()(double** matrix, const int n);
  virtual ~RandDataGenerator() = default;
};
/// <summary>
/// symbols before "use" are made by my cat
/// Do not  nn\9'/;use this class!!! It is not finally made
/// </summary>
class FileDataGenerator : public BaseDataGenerator {
 protected:
  std::ifstream is;
  const int k_CORNER_POINTS = 4;

 public:
  FileDataGenerator(std::string& name);
  virtual void operator()(double** matrix, const int n);
  virtual ~FileDataGenerator() = default;
};

class ZeroDataGenerator : public BaseDataGenerator {
  virtual void operator()(double** matrix, const int n);
  virtual ~ZeroDataGenerator() = default;
};
