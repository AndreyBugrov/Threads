#include <omp.h>

#include "num_solving.h"
double NumDirichlet::seq_gauss_zeidel(double** result) {
  double dmax;  // maximum change of u_ij. Become less in every do-iteration
  double temp;  // to find maximum change
  double dm;  // current sequation change of u_ij. Become different in any inner
              // for-iteration
  iter_num_ = 0;
  double t1 = omp_get_wtime();
  for (int i = 0; i < k_N + 2; i++) {
    for (int j = 0; j < k_N + 2; j++) {
      result[i][j] = network_[i][j];
    }
  }
  do {
    iter_num_++;
    dmax = 0;  // to make minimal dmax
    for (int i = 1; i < k_N + 1; i++)
      for (int j = 1; j < k_N + 1; j++) {
        temp = result[i][j];
        result[i][j] = 0.25 * (result[i - 1][j] + result[i + 1][j] +
                               result[i][j - 1] + result[i][j + 1]);  // -
        // h_ * h_ * f_x_y(i * h_, j * h_));
        dm = fabs(temp - result[i][j]);
        if (dmax < dm) dmax = dm;
      }
  } while (dmax > eps_);  // main requirement of iterative process
  double t2 = omp_get_wtime();
  return t2 - t1;
}
double NumDirichlet::gauss_zeidel_simple(double** result) {
  omp_lock_t dmax_lock;
  omp_init_lock(&dmax_lock);
  double dmax;  // maximum change of u_ij. become less in every do-iteration
  double temp;  // to find maximum change
  double
      d;  // current change of u_ij. Become different in any inner for-iteration
  iter_num_ = 0;
  double t1 = omp_get_wtime();
  for (int i = 0; i < k_N + 2; i++) {
    for (int j = 0; j < k_N + 2; j++) {
      result[i][j] = network_[i][j];
    }
  }
  do {
    iter_num_++;
    dmax = 0;  // to make minimal dmax
#pragma omp parallel for shared(result, dmax) private(temp, d)
    for (int i = 1; i < k_N + 1; i++) {
#pragma omp parallel for shared(result, dmax)  // private(j, temp, d)
      for (int j = 1; j < k_N + 1; j++) {
        temp = result[i][j];
        result[i][j] =
            0.25 * (result[i - 1][j] + result[i + 1][j] + result[i][j - 1] +
                    result[i][j + 1]);  //Цh * h * f[i][j]);
        d = fabs(temp - result[i][j]);
        omp_set_lock(&dmax_lock);
        if (dmax < d) dmax = d;
        omp_unset_lock(&dmax_lock);
      }  // end of nested parallel fragment
    }    // end of outer parallel fragment
  } while (dmax > eps_);
  double t2 = omp_get_wtime();
  return t2 - t1;
}
double NumDirichlet::gauss_zeidel_less_sync(double** result) {
  double dmax;  // maximum change of u_ij. Become less in every do-iteration
  double temp;  // to find maximum change
  double dm;    // current change of u_ij. Become different in any outer
                // for-iteration
  double d;     // current change of u_ij. Become different in any inner
                // for-iteration
  omp_lock_t dmax_lock;
  omp_init_lock(&dmax_lock);
  iter_num_ = 0;
  double t1 = omp_get_wtime();
  // without pragma (500 n matrix): 5.02, 5.12, 5.17, 5.00, 4.98, 5.04, 5.17
  // with pragma (500 n matrix): 4.99, 5.12, 4.91, 5.22, 5.08
  // so pragma shouldn't live
  //#pragma omp parallel for shared(network_, result, k_N) private(i)
  for (int i = 0; i < k_N + 2; i++) {
    for (int j = 0; j < k_N + 2; j++) {
      result[i][j] = network_[i][j];
    }
  }
  do {
    iter_num_++;
    dmax = 0;  // to make minimal dmax
#pragma omp parallel for shared(result, dmax) private(temp, d, dm)
    for (int i = 1; i < k_N + 1; i++) {
      dm = 0;  // to make minimal dm
      for (int j = 1; j < k_N + 1; j++) {
        temp = result[i][j];
        result[i][j] = 0.25 * (result[i - 1][j] + result[i + 1][j] +
                               result[i][j - 1] + result[i][j + 1]);  // -
        // h_ * h_ * f_x_y(i * h_, j * h_));
        d = fabs(temp - result[i][j]);
        if (dm < d) dm = d;
      }
      omp_set_lock(&dmax_lock);
      if (dmax < dm) dmax = dm;
      /*std::cout << "Number of threads in parallel section: " <<
      omp_get_num_threads() << "\n";

      std::cout << "current thread  = " << omp_get_num_procs() << "\n";
      return 3.14;*/
      omp_unset_lock(&dmax_lock);
    }
  } while (dmax > eps_);  // main requirement of iterative process
  double t2 = omp_get_wtime();
  return t2 - t1;
}
double NumDirichlet::gauss_jacoby(double** result) {
  double dmax;  // maximum change of u_ij. Become less in every do-iteration
  double temp;  // to find maximum change
  double dm;    // current change of u_ij. Become different in any outer
                // for-iteration
  double d;     // current change of u_ij. Become different in any inner
                // for-iteration
  omp_lock_t dmax_lock;
  omp_init_lock(&dmax_lock);
  iter_num_ = 0;
  double t1 = omp_get_wtime();
  double** preresult = new double*[k_N + 2];
  for (int i = 0; i < k_N + 2; i++) {
    preresult[i] = new double[k_N + 2];
  }
  for (int i = 0; i < k_N + 2; i++) {
    for (int j = 0; j < k_N + 2; j++) {
      preresult[i][j] = result[i][j] = network_[i][j];
    }
  }
  do {
    iter_num_++;
    dmax = 0;  // to make minimal dmax
#pragma omp parallel for shared(preresult, result, dmax) private(temp, d, dm)
    for (int i = 1; i < k_N + 1; i++) {
      dm = 0;  // to make minimal dm
      for (int j = 1; j < k_N + 1; j++) {
        temp = result[i][j];
        preresult[i][j] = 0.25 * (result[i - 1][j] + result[i + 1][j] +
                                  result[i][j - 1] + result[i][j + 1]);  // -
        // h_ * h_ * f_x_y(i * h_, j * h_));
        d = fabs(temp - preresult[i][j]);
        if (dm < d) dm = d;
      }
      omp_set_lock(&dmax_lock);
      if (dmax < dm) dmax = dm;
      omp_unset_lock(&dmax_lock);
    }
    for (int i = 1; i < k_N + 1; i++) {
      for (int j = 1; j < k_N + 1; j++) {
        result[i][j] = preresult[i][j];
      }
    }
  } while (dmax > eps_);  // main requirement of iterative process
  double t2 = omp_get_wtime();
  for (int i = 0; i < k_N + 2; i++) {
    delete[] preresult[i];
  }
  delete[] preresult;
  return t2 - t1;
}
double NumDirichlet::gauss_zeidel_wave(double** result) {
  double dmax;  // maximum change of u_ij. Become less in every do-iteration
  double temp;  // to find maximum change
  double* dm = new double[k_N + 1];  // current change of u_ij. Become different
                                     // in any outer for-iteration
  double d;  // current change of u_ij. Become different in any inner
             // for-iteration
  omp_lock_t dmax_lock;
  omp_init_lock(&dmax_lock);
  iter_num_ = 0;
  for (int i = 0; i < k_N + 2; i++) {
    for (int j = 0; j < k_N + 2; j++) {
      result[i][j] = network_[i][j];
    }
  }
  for (int i = 0; i < k_N + 1; i++) {
    dm[i] = 0;
  }
  double t1 = omp_get_wtime();
  do {
    iter_num_++;
    dmax = 0;  // to make minimal dmax
    for (int nx = 1; nx < k_N + 1; nx++) {
      dm[nx] = 0.0;
#pragma omp parallel for shared(result, nx, dm) private(temp, d)
      for (int i = 1; i < nx + 1; i++) {
        int j = nx + 1 - i;
        temp = result[i][j];
        result[i][j] = 0.25 * (result[i - 1][j] + result[i + 1][j] +
                               result[i][j - 1] + result[i][j + 1]);
        d = fabs(temp - result[i][j]);
        if (dm[i] < d) dm[i] = d;
      }
    }
    for (int nx = k_N - 1; nx > 0; nx--) {
#pragma omp parallel for shared(result, nx, dm) private(temp, d)
      for (int i = k_N - nx + 1; i < k_N + 1; i++) {
        int j = 2 * k_N - nx - i + 1;
        temp = result[i][j];
        result[i][j] =
            0.25 * (result[i - 1][j] + result[i + 1][j] + result[i][j - 1] +
                    result[i][j + 1]);  //Цh * h * f[i][j]);
        d = fabs(temp - result[i][j]);
        if (dm[i] < d) dm[i] = d;
      }  // конец параллельной области
    }
    // 25.04.23: the presence of pragma causes the program to slow down
    // experiment: n = 500 and eps = 0.01
    // middle of non-parallel: 3.372 s
    // middle of parallel: 3.538 s
    // all 5 launches non-parallel program was faster
    // pragma should not exist
    //#pragma omp parallel for shared(k_N, dmax, dm) private(i)
    for (int i = 1; i < k_N + 1; i++) {
      omp_set_lock(&dmax_lock);
      if (dmax < dm[i]) dmax = dm[i];
      // std::cout << "thread_num  = " << omp_get_num_threads() << "\n";
      omp_unset_lock(&dmax_lock);
    }

  } while (dmax > eps_);  // main requirement of iterative process
  delete[] dm;
  double t2 = omp_get_wtime();
  return t2 - t1;
}

double NumDirichlet::gauss_zeidel_block_wave(double** result) {
  double dmax;  // maximum change of u_ij. Become less in every do-iteration
  double temp;  // to find maximum change
  double* dm = new double[k_N + 1];  // current change of u_ij. Become different
                                     // in any outer for-iteration
  double d;  // current change of u_ij. Become different in any inner
             // for-iteration
  omp_lock_t dmax_lock;
  omp_init_lock(&dmax_lock);
  iter_num_ = 0;
  for (int i = 0; i < k_N + 2; i++) {
    for (int j = 0; j < k_N + 2; j++) {
      result[i][j] = network_[i][j];
    }
  }
  for (int i = 0; i < k_N + 1; i++) {
    dm[i] = 0;
  }
  double t1 = omp_get_wtime();
  const int n = (k_N) / omp_get_num_threads();
  const int block_num = omp_get_num_threads();
  // cout << "\nNum of Threads: " << omp_get_num_threads() << "\n";
  do {
    iter_num_++;
    dmax = 0;                                 // to make minimal dmax
    for (int nx = 0; nx < block_num; nx++) {  // количество блоков
#pragma omp parallel for shared(block_num, result, dm, nx) private(temp, d)
      for (int i = 1; i < nx + 1; i++) {
        int j = nx + 1 - i;
        temp = result[i][j];
        result[i][j] =
            0.25 * (result[i - 1][j] + result[i + 1][j] + result[i][j - 1] +
                    result[i][j + 1]);  //Цh * h * f[i][j]);
        d = fabs(temp - result[i][j]);
        if (dm[i] < d) dm[i] = d;
      }  // конец параллельной области
    }
    // затухание волны
    for (int nx = block_num - 2; nx > 0; nx--) {
#pragma omp parallel for shared(block_num, result, dm, nx) private(temp, d)
      for (int i = block_num - nx; i < block_num + 1; i++) {
        int j = 2 * (block_num - 1) - nx - i + 1;
        temp = result[i][j];
        result[i][j] =
            0.25 * (result[i - 1][j] + result[i + 1][j] + result[i][j - 1] +
                    result[i][j + 1]);  //Цh * h * f[i][j]);
        d = fabs(temp - result[i][j]);
        if (dm[i] < d) dm[i] = d;
      }  // конец параллельной области
    }
#pragma omp parallel for shared(dmax, dm)
    for (int i = 1; i < k_N + 1; i++) {
      omp_set_lock(&dmax_lock);
      if (dmax < dm[i]) dmax = dm[i];
      omp_unset_lock(&dmax_lock);
    }
  } while (dmax > eps_);  // main requirement of iterative process
  delete[] dm;
  double t2 = omp_get_wtime();
  return t2 - t1;
}