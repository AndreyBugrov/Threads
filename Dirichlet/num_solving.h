#pragma once
#include <time.h>  // time(NULL) in mt19937

#include <fstream>   // ifstream. Delete?
#include <iostream>  // i/o operations
#include <random>    // mt19937
#include <string>    // getting lines from user, file, script
#include <vector>    // for vector<string> der. Delete?

#include "data_generator.h"

using std::cin;
using std::cout;
using std::string;
using std::vector;

class NumDirichlet {
 public:
  static const int k_CORNER_POINTS = 4;

 protected:
  const int k_N;      // number of internal points (iterate from 1 to k_N+1)
  const double h_;    // step in network
  double** network_;  // should be stored in class-allocator
  double eps_;        // maximum permissible inaccuracy
  bool got_values_;   // true if get_from_user or get_from_script have already
                      // been called
 protected:
  BaseDataGenerator* generator_;

  void generate_frame(const vector<string>& der);
  void line_derivative(int i, int j, double a, double b);
  double f_x_y(
      double x,
      double y);  // ?: how can I create all functions as the second derivative
  void generate_inner_values();  // ?: we can't limit spread of values
  void parse_x_y_str(const string& str, double& a, double& b, bool der_of_x);
  void seq_gauss_zeidel();

 public:
  enum class Method {
    k_Seq = 0,
    k_GaussZeidelSimple = 1,
    k_GaussZeidelLessSync = 2,
    k_GaussJacobi = 3,
    k_GaussZeidelWave = 4,
    k_GaussZeidelBlockWave = 5
  };

  void get_from_user();
  void get_from_script();
  void operator()(Method method, double** result);

  // friend std::istream& operator>>(
  //    std::istream& is, NumDirichlet& num_dir);  // may be not necessary

  NumDirichlet(BaseDataGenerator* generator, int k_N = 10, double eps_ = 0.01)
      : generator_(generator),
        k_N(k_N),
        eps_(eps_),
        h_(1.0 / k_N),
        got_values_(false) {
    network_ = new double*[k_N + 2];
    for (int i = 0; i < k_N + 2; i++) {
      network_[i] = new double[k_N + 2];
    }
  }

  ~NumDirichlet() {
    for (int i = 0; i < k_N + 2; i++) {
      delete[] network_[i];
    }
    delete[] network_;
  }
};
