#include <iostream> // i/o operations
#include <chrono> // chrono functions
#include <string> // stoi è stod
#include <thread> // concurrent version
using std::thread;
using std::cout;

// for fast type changes
typedef double long_type;

void get_params(int& n,int&m, int& thread_num,bool& is_linear_matrix,bool& is_serial)//,int& serial_number)
{
    std::cout << "n: ";
    std::cin >> n;
    cout << "m: ";
    std::cin >> m;
    std::cout << "Thread number: ";
    std::cin >> thread_num;
    std::cout << "If you want to see linear matrix, press l. Else press anything ";
    char tmp;
    std::cin >> tmp;
    is_linear_matrix = (tmp == 'l') ? true : false;
    cout << "If the experiment is serial, press s. Else press anything ";
    std::cin >> tmp;
    is_serial = (tmp == 's') ? true : false;
    /*if (is_serial) {
        cout << "Serial number: ";
        std::cin >> serial_number;
    }
    else {
        serial_number = 1;
    }*/
}
void result_matrix_zeroing(long_type* result_matrix, int n)
{
    for (int i = 0; i < n; i++) {
        result_matrix[i] = long_type(0);
    }
}


void random_fill_vector(long_type* vector, int m)
{
    for (int i = 0; i < m; i++) {
        if (i % 7 == 0) {
            vector[i] = long_type(m) - i;
        }
        else {
            vector[i] = long_type(i);
        }
    }
}
void trivial_fill_vector(long_type* vector, int m)
{
    for (int i = 0; i < m; i++) {
        vector[i] = long_type(1);
    }
}

void random_fill_common_matrix(long_type** matrix, int n, int m)
{
    for (int i = 0; i < n; i++) {
        if (i % 13 == 0) {
            for (int j = 0; j < m; j++) {
                matrix[i][j] = long_type(-i) - j;
            }
        }
        else {
            for (int j = 0; j < m; j++) {
                matrix[i][j] = long_type(i) + m - j;
            }
        }
    }
}
void trivial_fill_common_matrix(long_type** matrix, int n, int m)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix[i][j] = long_type(1);
        }
    }
}

void random_fill_linear_matrix(long_type* matrix, int n, int m) 
{
    for (int i = 0; i < n; i++) {
        if (i % 13 == 0) {
            for (int j = 0; j < m; j++) {
                matrix[i * m + j] = long_type(-i) - j;
            }
        }
        else {
            for (int j = 0; j < m; j++) {
                matrix[i * m + j] = long_type(i) + m - j;
            }
        }
    }
}
void trivial_fill_linear_matrix(long_type* matrix, int n, int m)
{
    int num = n * m;
    for (int i = 0; i < num; i++) {
        matrix[i] = long_type(1);
    }
}

void common_multiplying(long_type** matrix, long_type* vector, long_type* result_matrix, int n, int m)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            result_matrix[i] += matrix[i][j] * vector[j];
        }
    }
}
void linear_multiplying(long_type* matrix, long_type* vector, long_type* result_matrix, int n, int m)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            result_matrix[i] += matrix[i * m + j] * vector[j];
        }
    }
}

long_type max_inaccuracy(long_type* result_matrix1, long_type* result_matrix2,int n)
{
    long_type max_i = abs(result_matrix1[0] - result_matrix2[0]);
    for (int i = 1; i < n; i++) {
        long_type tmp = abs(result_matrix1[i] - result_matrix2[i]);
        if (tmp > max_i) {
            max_i = tmp;
        }
    }
    return max_i;
}


void calc_common_matrix(long_type* vector, long_type* result_matrix, int n, int m, int thread_num, bool trivial,bool is_serial)
{
    long_type** matrix;
    matrix = new long_type * [n];
    long_type* result_matrix_2 = new long_type[n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new long_type[m];
    }
    result_matrix_zeroing(result_matrix, n);
    if (trivial) {
        trivial_fill_common_matrix(matrix, n, m);
    }
    else {
        random_fill_common_matrix(matrix, n, m);
    }
    random_fill_vector(vector, m);
    auto begin_seq = std::chrono::steady_clock::now();
    common_multiplying(matrix, vector, result_matrix, n, m);
    auto end_seq = std::chrono::steady_clock::now();
    auto elapsed_seq = std::chrono::duration_cast<std::chrono::microseconds>(end_seq - begin_seq);
    if (is_serial) {
        cout << elapsed_seq.count() << "\n";
    }
    else {
        std::cout << "Sequantial time: " << elapsed_seq.count() << " microseconds\n";
    }
    //-----------------------------------------------------------------------------------------------
    result_matrix_zeroing(result_matrix_2, n);
    //-----------------------------------------------------------------------------------------------
    int part = n / thread_num;
    auto begin_con = std::chrono::steady_clock::now();
    thread* thr = new thread[thread_num];
    for (int i = 0; i < thread_num - 1; i++) {
        thr[i] = thread(common_multiplying, matrix + part * i, vector, result_matrix_2 + part * i, part, m);
    }

    int reminder = n % thread_num;
    int adding = part * (thread_num - 1);
    thr[thread_num - 1] = thread(common_multiplying, matrix + adding, vector, result_matrix_2 + adding, part + reminder, m);
    for (int i = 0; i < thread_num; i++) {
        thr[i].join();
    }
    auto end_con = std::chrono::steady_clock::now();
    auto elapsed_con = std::chrono::duration_cast<std::chrono::microseconds>(end_con - begin_con);
    if (is_serial) {
        cout << elapsed_con.count() << "\n";
        cout << max_inaccuracy(result_matrix, result_matrix_2, n) << "\n";
        cout << double(elapsed_seq.count()) / elapsed_con.count() << "\n";
    }
    else {
        /*if (trivial) {
            cout << "Oops!" << "\n";
        }
        else {
            cout << "Ho-ho-ho" << "\nyes\n";
        }
        for (int i = 0; i < n; i++) {
            cout << "Sequantial[" << i << "] = " << result_matrix[i] << "\n";
        }
        for (int i = 0; i < n; i++) {
            cout << "Concurrent[" << i << "] = " << result_matrix_2[i] << "\n";
        }
        for (int i = 0; i < n; i++) {
            cout << "Difference[" << i << "] = " << double(result_matrix[i]) - result_matrix_2[i] << "\n";
        }*/
        cout << "Concurrent time: " << elapsed_con.count() << " microseconds\n" << "\n";
        cout << "Inaccuracy: " << max_inaccuracy(result_matrix, result_matrix_2, n) << "\n";
        cout << "Speedup: " << double(elapsed_seq.count()) / elapsed_con.count();
    }
}
void calc_linear_matrix(long_type* vector, long_type* result_matrix, int n, int m, int thread_num,bool trivial,bool is_serial)
{
    long_type* matrix = new long_type[n * m];
    long_type* result_matrix_2 = new long_type[n];
    result_matrix_zeroing(result_matrix, n);
    if (trivial) {
        trivial_fill_linear_matrix(matrix, n, m);
    }
    else {
        random_fill_linear_matrix(matrix, n, m);
    }
    random_fill_vector(vector, m);
    auto begin_seq = std::chrono::steady_clock::now();
    linear_multiplying(matrix, vector, result_matrix, n, m);
    auto end_seq = std::chrono::steady_clock::now();
    auto elapsed_seq = std::chrono::duration_cast<std::chrono::microseconds>(end_seq - begin_seq);
    if (is_serial) {
        std::cout << elapsed_seq.count() << "\n";
    }
    else {
        std::cout << "Sequantial time: " << elapsed_seq.count() << " microseconds\n";
    }
    //-----------------------------------------------------------------------------------------------
    result_matrix_zeroing(result_matrix_2, n);
    //-----------------------------------------------------------------------------------------------
    int n_part = n / thread_num; // separating by the strings
    int full_part = m * n_part; // for right jumping in elements of linear matrix
    auto begin_con = std::chrono::steady_clock::now();
    thread* thr = new thread[thread_num];
    for (int i = 0; i < thread_num - 1; i++) {
        thr[i] = thread(linear_multiplying, matrix + full_part * i, vector, result_matrix_2 + n_part * i, n_part, m);
    }

    int reminder = n % thread_num;
    int n_adding = n_part * (thread_num - 1); // jumping in result matrix
    int full_adding = full_part * (thread_num - 1); // jumping in linear matrix
    thr[thread_num - 1] = thread(linear_multiplying, matrix + full_adding, vector, result_matrix_2 + n_adding, n_part + reminder, m);
    for (int i = 0; i < thread_num; i++) {
        thr[i].join();
    }
    auto end_con = std::chrono::steady_clock::now();
    auto elapsed_con = std::chrono::duration_cast<std::chrono::microseconds>(end_con - begin_con);
    if (is_serial) {
        cout << elapsed_con.count() << "\n";
        cout << max_inaccuracy(result_matrix, result_matrix_2, n) << "\n";
        cout << double(elapsed_seq.count()) / elapsed_con.count() << "\n";
    }
    else {
        /*if (trivial) {
            cout << "Oops!" << "\n";
        }
        else {
            cout << "Ho-ho-ho" << "\nyes\n";
        }
        for (int i = 0; i < n; i++) {
            cout << "Sequantial[" << i << "] = " << result_matrix[i] << "\n";
        }
        for (int i = 0; i < n; i++) {
            cout << "Concurrent[" << i << "] = " << result_matrix_2[i] << "\n";
        }
        for (int i = 0; i < n; i++) {
            cout << "Difference[" << i << "] = " << double(result_matrix[i]) - result_matrix_2[i] << "\n";
        }*/
        cout << "Concurrent time: " << elapsed_con.count() << " microseconds\n" << "\n";
        cout << "Inaccuracy: " << max_inaccuracy(result_matrix, result_matrix_2, n) << "\n";
        cout << "Speedup: " << double(elapsed_seq.count()) / elapsed_con.count();
    }
}
int main(int argc, char* argv[])
{
    int n, m, thread_num;
    bool is_serial = false, is_linear_matrix = false;
    if (argc < 6) {
        int serial_number;
        get_params(n, m, thread_num,is_linear_matrix,is_serial);
        std::cout << "\n";
    }
    else {
        n = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        thread_num = std::stoi(argv[3]);
        is_serial = (argv[4][0] == 's') ? true : false;
        if (argv[5][0] == 'l') {
            is_linear_matrix = true;
        }
    }
    long_type* vector, * result_matrix;
    result_matrix = new long_type[n];
    vector = new long_type[m];
    //--------------------------------------------
    const bool trivial = false; // I decided to set the value manually
    //--------------------------------------------
    if (is_linear_matrix) {
        calc_linear_matrix(vector, result_matrix, n, m, thread_num, trivial, is_serial);
    }
    else {
        calc_common_matrix(vector, result_matrix, n, m, thread_num, trivial, is_serial);
    }
}