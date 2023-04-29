#include "num_solving.h"

/*
-200y+100
200y-100
-200x+100
200x-100

*/

/*
100y
-100y+100
100x
-100x+100

*/

/*
N=100
eps = 0.1
iter = 147209 - DEBUG
iter = 207-223 - RELEASE
NEEDS = 210
Zero and Random
*/

void create_network_array(double**& network, int& k_N, double& eps) {
  cout << "Enter N: ";
  cin >> k_N;
  network = new double*[k_N + 2];
  for (int i = 0; i < k_N + 2; i++) {
    network[i] = new double[k_N + 2];
  }
  cout << "Enter epsilon: ";
  cin >> eps;
  getchar();
}

int main(int argc, char* argv[]) {
  int k_N = 10;
  double eps = 0.01;
  double** network = nullptr;
  // somehow define the mode
  switch (argc) {
    case 1: {  // common user input
      create_network_array(network, k_N, eps);
      BaseDataGenerator* generator =\
           new RandDataGenerator(-100, 100);  //(-100.0, 100.0);
          //new ZeroDataGenerator();
      NumDirichlet nd(generator, k_N, eps);
      nd.get_from_user();
      cout << "1 - Gauss-Zeidel Sequential\n";
      cout << "2 - Gauss-Zeidel Simple\n";
      cout << "3 - Gauss-Zeidel Less Synchronization\n";
      cout << "4 - Gauss-Jacobi\n";
      cout << "5 - Gauss-Zeidel Wave\n";
      cout << "6 - Gauss-Zeidel Block Wave\n";
      cout << "7 - Change k_N and epsilon\n";
      cout << "0 - Exit\n";
      int choice;
      NumDirichlet::Method method;
      do {
        cin >> choice;
        switch (choice) {
          case 0:
            break;
          case 1:
            method = NumDirichlet::Method::k_Seq;
            break;
          case 2:
            method = NumDirichlet::Method::k_GaussZeidelSimple;
            break;
          case 3:
            method = NumDirichlet::Method::k_GaussZeidelLessSync;
            break;
          case 4:
            method = NumDirichlet::Method::k_GaussJacobi;
            break;
          case 5:
            method = NumDirichlet::Method::k_GaussZeidelWave;
            break;
          case 6:
            method = NumDirichlet::Method::k_GaussZeidelBlockWave;
            break;
          case 7: {
            for (int i = 0; i < k_N + 2; i++) {
              delete network[i];
            }
            delete network;
            create_network_array(network, k_N, eps);
            nd.recreate(k_N, eps);
          } break;
          default:
            cout << "Wrong answer\n";
            break;
        }
        if (choice && choice != 7) {
          double time = nd(method, network);
          if (k_N < 20)
            for (int i = 0; i < k_N + 2; i++) {
              for (int j = 0; j < k_N + 2; j++) {
                cout << network[i][j] << ' ';
              }
              cout << '\n';
            }
          cout << "Iteration number: " << nd.iter_num();
          cout << "\nTime: " << time << "\n";
        }
      } while (choice);
    } break;
      return 0;
    case 4: {
      if (argv[3] == "s") {
        // script mode (minimum output)
      }
    }  // !: without break;
    case 3: {
      // more pro user input
      eps = stod(std::string(argv[1]));
    }  // !: without break;
    case 2: {
      // pro user input
      int k_N = stoi(std::string(argv[0]));
    } break;
    default:
      break;
  }
  // NumDirichlet nd(k_N, eps);
  // other things
  return 0;
}