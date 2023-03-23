#include "num_solving.h"
void NumDirichlet::get_from_user() {
  // get global second derivative
  vector<string> der(k_CORNER_POINTS);
  cout << "Enter derivative in: ";
  for (int i = 0; i < k_CORNER_POINTS; i++) {
    i / 2 == 0 ? cout << "x = " : cout << "y = ";
    cout << i % 2 << ": ";
    std::getline(cin, der[i], '\n');
  }
  generate_frame(der);
  generate_inner_values();
  got_values_ = true;
}

void NumDirichlet::generate_inner_values() {
 /* std::mt19937 engine(time(NULL));
  generator_->operator()(network_ + 1, k_N);*/
  //for (int i = 1; i < k_N + 1; i++) {
  //  for (int j = 1; j < k_N + 1; j++) {
  //  }
  //}
}

void parse_x_y_str(const string& str, double& a, double& b, bool der_of_x) {
  using std::stod;
  if (str.size() == 1) {
    if (der_of_x && str[0] == 'y' || !der_of_x && str[0] == 'x') {
      a = 1;
      b = 0;
      return;
    }
    b = stod(str);
    a = 0;
    return;
  }
  string sub = str.substr(1);  // substring (str without 0-th element)
  size_t delim = sub.find_first_of('+');  // delimiter
  const size_t not_found = string::npos;
  if (delim == not_found) {
    delim = sub.find_first_of('-');
  }
  if (delim == not_found) {
    der_of_x ? delim = str.find_first_of('y') : delim = str.find_first_of('x');
    if (delim == not_found) {
      a = 0;
      b = stod(str);
      return;
    }
    string tmp_sub = str.substr(0, str.size() - 1);
    if ((tmp_sub[0] == '+' || tmp_sub[0] == '-') && tmp_sub.size() > 1 ||
        tmp_sub[0] != '+' && tmp_sub[0] != '-') {
      a = stod(tmp_sub);
      b = 0;
      return;
    }
  }
  string sub1 = str.substr(0, delim);
  switch (sub1.size()) {
    case 0: {
      if (der_of_x) {
        size_t tmp = str.find_first_of('y');
        if (tmp == not_found) {
          a = 0;
        } else {
          a = 1;
        }
      }
    } break;
    case 1: {
      switch (str[0]) {
        case '+':
          a = 1;
          break;
        case '-':
          a = -1;
          break;
        default:
          a = stod(sub1);
          break;
      }
    } break;
    default:
      a = stod(sub1);
      break;
  }
  string sub2 = sub.substr(delim);
  switch (sub2.size()) {
    case 0: {
      b = 0;
    } break;
    default:
      b = stod(sub2);
      break;
  }
}

void NumDirichlet::generate_frame(const vector<string>& der) {
  double a, b;
  for (int i = 0; i < k_CORNER_POINTS / 2; i++) {
    parse_x_y_str(der[i], a, b, true);
    line_derivative(i, -1, a, b);
  }
  for (int j = k_CORNER_POINTS / 2; j < k_CORNER_POINTS; j++) {
    parse_x_y_str(der[j], a, b, true);
    line_derivative(-1, j, a, b);
  }
}
void NumDirichlet::line_derivative(int i, int j, double a, double b) {
  if (j < 0) {
    for (int j = 0; j < k_N + 2; j++) {
      network_[i][j] = a * j + b;
    }
  } else {
    for (int i = 0; i < k_N + 2; i++) {
      network_[j][i] = a * i + b;
    }
  }
}

void NumDirichlet::operator()(NumDirichlet::Method method, double** result) {
  if (!got_values_) {
    throw string("Values did not been got");
  }
  switch (method) {
    case NumDirichlet::Method::k_Seq:
      seq_gauss_zeidel();
      break;
    case NumDirichlet::Method::k_GaussZeidelSimple:
      break;
    case NumDirichlet::Method::k_GaussZeidelLessSync:
      break;
    case NumDirichlet::Method::k_GaussJacobi:
      break;
    case NumDirichlet::Method::k_GaussZeidelWave:
      break;
    case NumDirichlet::Method::k_GaussZeidelBlockWave:
      break;
    default:
      break;
  }
  // This can be function assign_result()
  for (int i = 0; i < k_N + 2; i++) {
    for (int j = 0; j < k_N + 2; j++) {
      result[i][j] = network_[i][j];
    }
  }
}

double NumDirichlet::f_x_y(double x, double y) { return 0; }
