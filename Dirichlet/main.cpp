#include "num_solving.h"

/*
-200y+100
200y-100
-200x+100
200x-100

*/

int main(int argc, char* argv[]) {
  int k_N = 10;
  double eps = 0.01;
  // somehow define the mode
  switch (argc) {
    case 1: {  // common user input
      cout << "Enter N: ";
      cin >> k_N;
      double** network = new double*[k_N + 2];
      for (int i = 0; i < k_N + 2; i++) {
        network[i] = new double[k_N + 2];
      }
      cout << "Enter epsilon: ";
      cin >> eps;
      getchar();
      BaseDataGenerator* generator =
          new RandDataGenerator(-1, 1);  //(-100.0, 100.0);
      NumDirichlet nd(generator, k_N, eps);
      nd.get_from_user();
      nd(NumDirichlet::Method::k_Seq, network);
      for (int i = 0; i < k_N + 2; i++) {
        for (int j = 0; j < k_N + 2; j++) {
          cout << network[i][j] << ' ';
        }
        cout << '\n';
      }
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
  //NumDirichlet nd(k_N, eps);
  // other things
  return 0;
}