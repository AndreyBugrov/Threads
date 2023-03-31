#include <omp.h>

#include "num_solving.h"
double NumDirichlet::seq_gauss_zeidel() {
  double dmax;  // maximum change of u_ij. become less in every do-iteration
  double temp;  // to find maximum change
  double dm;    // current change of u_ij. Become different in any for-iteration
  iter_num_ = 0;
  double t1 = omp_get_wtime();
  do {
    iter_num_++;
    dmax = 0;
    for (int i = 1; i < k_N + 1; i++)
      for (int j = 1; j < k_N + 1; j++) {
        temp = network_[i][j];
        network_[i][j] = 0.25 * (network_[i - 1][j] + network_[i + 1][j] +
                                 network_[i][j - 1] + network_[i][j + 1]);  // -
        // h_ * h_ * f_x_y(i * h_, j * h_));
        dm = fabs(temp - network_[i][j]);
        if (dmax < dm) dmax = dm;
      }
  } while (dmax > eps_);  // main requirement of iterative process
  double t2 = omp_get_wtime();
  return t2 - t1;
}
double NumDirichlet::gauss_zeidel_simple() {
  omp_lock_t dmax_lock;
  omp_init_lock(&dmax_lock);
  double dmax;  // maximum change of u_ij. become less in every do-iteration
  double temp;  // to find maximum change
  double dm;    // current change of u_ij. Become different in any for-iteration
  double d;
  iter_num_ = 0;
  double t1 = omp_get_wtime();
  do {
    iter_num_++;
    dmax = 0;  // максимальное изменение значений u
#pragma omp parallel for shared(u, n, dmax) private(i, temp, d)
    for (int i = 1; i < k_N + 1; i++) {
#pragma omp parallel for shared(u, n, dmax) private(j, temp, d)
      for (int j = 1; j < k_N + 1; j++) {
        temp = network_[i][j];
        network_[i][j] = 0.25 * (network_[i - 1][j] + network_[i + 1][j] +
                                 network_[i][j - 1] +
                                 network_[i][j + 1]);  //–h * h * f[i][j]);
        d = fabs(temp - network_[i][j]);
        omp_set_lock(&dmax_lock);
        if (dmax < d) dmax = d;
        omp_unset_lock(&dmax_lock);
      }  // конец вложенной параллельной области
    }    // конец внешней параллельной области
  } while (dmax > eps_);
  double t2 = omp_get_wtime();
  return t2 - t1;
}
