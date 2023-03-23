#include "num_solving.h"
void NumDirichlet::seq_gauss_zeidel() {
  double dmax;
  double temp;
  double dm;
  do {
    dmax = 0;  // максимальное изменение значений u_ij
    for (int i = 1; i < k_N + 1; i++)
      for (int j = 1; j < k_N + 1; j++) {
        temp = network_[i][j];
        network_[i][j] = 0.25 * (network_[i - 1][j] + network_[i + 1][j] +
                                 network_[i][j - 1] + network_[i][j + 1] -
                                 h_ * h_ * f_x_y(i * h_, j * h_));
        dm = fabs(temp - network_[i][j]);
        if (dmax < dm) dmax = dm;
      }
  } while (dmax > eps_);  // главное условие итерационного процесса
}