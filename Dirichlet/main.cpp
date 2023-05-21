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

void create_array(double**& network, int element_num) {
  network = new double*[element_num];
  for (int i = 0; i < element_num; i++) {
    network[i] = new double[element_num];
  }
}

void create_network_array(double**& network, int& k_N, double& eps) {
  cout << "Enter N: ";
  cin >> k_N;
  create_array(network, k_N + 2);
  cout << "Enter epsilon: ";
  cin >> eps;
  getchar();
}
void print_algorithms() {
  cout << "1 - Gauss-Zeidel Sequential\n";
  cout << "2 - Gauss-Zeidel Simple\n";
  cout << "3 - Gauss-Zeidel Less Synchronization\n";
  cout << "4 - Gauss-Jacobi\n";
  cout << "5 - Gauss-Zeidel Wave\n";
  cout << "6 - Gauss-Zeidel Block Wave\n";
  cout << "7 - Change k_N and epsilon\n";
  cout << "0 - Exit\n";
}
/// <summary>
///
/// </summary>
/// <param name="network">: сетка для послдеовательного результата </param>
/// <param name="k_N">: </param>
/// <param name="eps"></param>
/// <param name="nd"></param>
/// <param name="for_user"></param>
/// <param name="choice"></param>

void calc_mode(double**& network, int& k_N, double& eps, NumDirichlet& nd,
               bool for_user, int choice) {
  NumDirichlet::Method method;
  // to compare results from diffrent mods
  bool seq_was_performed = false;
  double** seq_network;
  create_array(seq_network, k_N + 2);
  do {
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
          delete[] network[i];
          delete[] seq_network[i];
        }
        delete[] network;
        delete[] seq_network;
        create_network_array(network, k_N, eps);
        create_array(seq_network, k_N + 2);
        nd.recreate(k_N, eps);
        seq_was_performed = false;
      } break;
      default:
        cout << "Wrong answer\n";
        choice = 7;
        break;
    }
    if (choice && choice != 7) {
      double time;
      double difference = 0;
      if (for_user) {
        if (method == NumDirichlet::Method::k_Seq) {
          time = nd(method, seq_network);
          seq_was_performed = true;
        } else {
          time = nd(method, network);
          if (seq_was_performed) {
            difference = diff(network, seq_network, k_N);
          }
        }
        if (k_N < 20)
          if (method == NumDirichlet::Method::k_Seq) {
            for (int i = 0; i < k_N + 2; i++) {
              for (int j = 0; j < k_N + 2; j++) {
                cout << seq_network[i][j] << ' ';
              }
              cout << '\n';
            }
          } else {
            for (int i = 0; i < k_N + 2; i++) {
              for (int j = 0; j < k_N + 2; j++) {
                cout << network[i][j] << ' ';
              }
              cout << '\n';
            }
          }
        cout << "Iteration number: " << nd.iter_num();
        cout << "\nTime: " << time << "\n";
        if (seq_was_performed) {
          std::cout << "Max difference from sequetial method: " << difference
                    << "\n";
        }
      } else {
        if (method == NumDirichlet::Method::k_Seq) {
          time = nd(method, network);
        } else {
          nd(NumDirichlet::Method::k_Seq, seq_network);
          time = nd(method, network);
          difference = diff(network, seq_network, k_N);
        }
        //string method_name;
        //switch (method) {
        //  case NumDirichlet::Method::k_Seq:
        //    method_name = "Последовательный Гаусса-Зейделя";
        //    break;
        //  case NumDirichlet::Method::k_GaussZeidelSimple:
        //    method_name = "Простой Гаусса-Зейделя";
        //    break;
        //  case NumDirichlet::Method::k_GaussZeidelLessSync:
        //    method_name = "Гаусса-Зейделя быстрый";
        //    break;
        //  case NumDirichlet::Method::k_GaussJacobi:
        //    method_name = "Гаусса-Якоби";
        //    break;
        //  case NumDirichlet::Method::k_GaussZeidelWave:
        //    method_name = "Гаусса-Якоби  волновой";
        //    break;
        //  case NumDirichlet::Method::k_GaussZeidelBlockWave:
        //    method_name = "Гаусса-Якоби блочный волновой";
        //    break;
        //  default:
        //    throw "a";
        //    break;
        //}
        cout << k_N << "\n"
             << nd.iter_num() << "\n"
             << time << "\n"
             << difference << "\n";
      }
    }
    if (for_user) cin >> choice;
  } while (for_user && choice);
  for (int i = 0; i < k_N + 2; i++) {
    delete[] network[i];
    delete[] seq_network[i];
  }
  delete[] network;
  delete[] seq_network;
}

////////////////////////////////////////////////////////////
void script_calc() {
  // ? should we get rid of seq method output by using time_seq / time_par
  //...
}

int main(int argc, char* argv[]) {
  int k_N = 10;
  double eps = 0.01;
  double** network = nullptr;
  setlocale(LC_ALL, "Russian");
  switch (argc) {
    case 1: {  // common user input
      create_network_array(network, k_N, eps);
      BaseDataGenerator* generator;
      int choice;
      cout << "Enter type of generator:\n";
      cout << "1 - Random generator\n";
      cout << "2 - Zero generator\n";
      cin >> choice;
      getchar();
      if (choice == 1) {
        generator = new RandDataGenerator(-100, 100);
      } else {
        if (choice == 2) {
          generator = new ZeroDataGenerator();
        } else {
          return -1;
        }
      }
      NumDirichlet nd(generator, k_N, eps);
      nd.get_from_user();
      //string frame[4] = {"-200y+100", "200y-100", "-200x+100", "200x-100"};
      //nd.get_from_script(frame[0], frame[1], frame[2], frame[3]);
      print_algorithms();
      cin >> choice;
      //calc_mode(network, k_N, eps, nd, false, choice);
      calc_mode(network, k_N, eps, nd, true, choice);
    } break;
      return 0;
    case 9: {
      // script mode (minimum output)
      k_N = stoi(std::string(argv[1]));
      eps = stod(std::string(argv[2]));
      BaseDataGenerator* generator;
      if (argv[3] == "rand") {
        generator = new RandDataGenerator(-100, 100);
      } else {
        generator = new ZeroDataGenerator();
      }
      create_array(network, k_N + 2);
      NumDirichlet nd(generator, k_N, eps);
      int choice = stoi(string(argv[4]));
      //for (int i = 1; i < 9; i++) {
      //  std::cout << argv[i] << "\n";
      //}
      string frame[4];
      for (int i = 0; i < 4; i++) {
        frame[i] = string(argv[i + 5]);
      }
      nd.get_from_script(frame[0], frame[1], frame[2], frame[3]);
      calc_mode(network, k_N, eps, nd, false, choice);
    } break;
    case 3: {
      // pro user input
      k_N = stoi(std::string(argv[1]));
      eps = stod(std::string(argv[2]));
      network = new double*[k_N + 2];
      for (int i = 0; i < k_N + 2; i++) {
        network[i] = new double[k_N + 2];
      }
      BaseDataGenerator* generator;
      int choice;
      cout << "Enter type of generator:\n";
      cout << "1 - Random generator\n";
      cout << "2 - Zero generator\n";
      cin >> choice;
      getchar();
      if (choice == 1) {
        generator = new RandDataGenerator(-100, 100);
      } else {
        if (choice == 2) {
          generator = new ZeroDataGenerator();
        } else {
          return -1;
        }
        NumDirichlet nd(generator, k_N, eps);
        nd.get_from_user();
        print_algorithms();
        int choice;
        cin >> choice;
        calc_mode(network, k_N, eps, nd, true, choice);
      }
    } break;
    default:
      return -1;
      break;
  }
  return 0;
}