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
  int k_N;            // number of internal points (iterate from 1 to k_N+1)
  double h_;          // step in network
  double** network_;  // should be stored in class-allocator
  double eps_;        // maximum permissible inaccuracy
  bool got_values_;   // true if get_from_user or get_from_script have already
                      // been called
  size_t iter_num_;
  BaseDataGenerator* generator_;

 protected:
  void generate_frame(const vector<string>& der);
  void line_derivative(int i, int j, double a, double b);
  double f_x_y(double x, double y);
  void generate_inner_values();
  void parse_x_y_str(const string& str, double& a, double& b, bool der_of_x);

  // theese methods return time
  // result is matrix which size is k_N+2 x k_N+2

  double seq_gauss_zeidel(double** result);  // sequential
  double gauss_zeidel_simple(
      double** result);  // slow because of synchronization
  double gauss_zeidel_less_sync(double** result);  // incorrect
  double gauss_jacoby(double** result);  // needs extra memory (k_N x k_N), is
                                         // not equal seq algorithm
  double gauss_zeidel_wave(double** result);
  double gauss_zeidel_block_wave(double** result);

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
  void get_from_script(string x_0, string x_1, string y_0, string y_1);
  double operator()(
      const Method& method,
      double** result);  // returns time, result is in result matrix
  size_t iter_num() { return iter_num_; }
  void clear();
  void recreate(const size_t& new_k_N, double eps);

  // friend std::istream& operator>>(
  //    std::istream& is, NumDirichlet& num_dir);  // may be not necessary

  // NumDirichlet(NumDirichlet&& nd)
  //    : k_N(nd.k_N),
  //      eps_(nd.eps_),
  //      h_(nd.h_),
  //      got_values_(got_values_),
  //      iter_num_(nd.iter_num_) {
  //  generator_ = nullptr;
  //  std::swap(generator_, nd.generator_);
  //  network_ = nullptr;
  //  std::swap(network_, nd.network_);
  //  nd.got_values_ = false;
  //}
 private:
  NumDirichlet(NumDirichlet&& nd);

 public:
  NumDirichlet(BaseDataGenerator* generator, int k_N = 10, double eps_ = 0.01)
      : generator_(generator),
        k_N(k_N),
        eps_(eps_),
        h_(1.0 / (k_N + 1)),
        got_values_(false),
        iter_num_(0) {
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

double diff(double** network, double** seq_network, int k_N);
