#include <thread>
#include <vector>
#include <iostream>
#include <chrono>
#include <string> 
using std::thread;
/// 1. Input from cmd +
/// 2. Print difference between numbers +
/// 3. Multiplying of matrixes and vectors
/// 


//FINALLY!!!
//Element number : 250000000
//Thread number : 25
//The linear time : 2 363 538 microseconds
//Linear result : 2.20353e+24
//The creating time : 30120 microseconds
//The join time : 1 554 104 microseconds
//The summing time : 0 microseconds
//Concurrent result : 2.20353e+24
//Diffrence between linear and concurrent results : 6.24918e+11
//Diffrence between linear and concurrent time : 779313 microseconds

// for fast type changes
typedef double long_type;
// Merging all buf elements into one sum (into result value)
void buf_sum(long_type* buf, size_t size, long_type& result) 
{
    for (int i = 0; i < size; i++) {
        result += buf[i];
    }
}

void buf_mult_and_sum(long_type* beginx, long_type* beginy, size_t size, long_type& result) 
{
    for (int i = 0; i < size; i++) {
        long_type tmp = beginx[i] * beginy[i];
       // tmp *= sin(beginx[i]) * sin(beginx[i]) + cos(beginx[i]) * cos(beginx[i]);
        result += tmp;
    }
}

void get_elem_and_threads(int& elem_num, int& thread_num)
{
    std::cout << "Element number: ";
    std::cin >> elem_num;
    std::cout << "Thread number: ";
    std::cin >> thread_num;
}

void fill_vectors(long_type* beginx, long_type* beginy, size_t size)
{
    for (int i = 0; i < size; i++) {
        long_type element = long_type(i);
        if (i % 13 == 0) {
            element *= long_type(-1);
            beginy[i] = long_type(size) + element;
        }
        else {
            beginy[i] = long_type(size) - element;
        }
        beginx[i] = long_type(element);
    }
}

int main(int argc, char* argv[])
{
   /* 
    auto begin = std::chrono::steady_clock::now();
    getchar();
    auto end = std::chrono::steady_clock::now();

    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    */
    int elem_num, thread_num;
    if (argc < 2) {
        get_elem_and_threads(elem_num, thread_num);
    }
    else {
        elem_num = std::stoi(argv[1]);
        thread_num = std::stoi(argv[2]);
    }
    long_type* first = new long_type[elem_num]; // first vector
    long_type* second = new long_type[elem_num]; // second vector
    long_type* result = new long_type[thread_num]; // result of summing of every thread
    long_type total_result1 = 0.0; // is created by one thread
    long_type total_result2 = 0.0;
    fill_vectors(first, second, elem_num);
    for (int i = 0; i < thread_num; i++) {
        result[i] = 0.0;
    }
    auto begin = std::chrono::steady_clock::now();
    buf_mult_and_sum(first, second, elem_num, std::ref(total_result1));
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    std::cout << "The linear time: " << elapsed_ms.count() << " microseconds\n";
    std::cout << "Linear result: " << total_result1 << "\n";

    //------------------------------------------------------------------------------------------------

    auto begin1 = std::chrono::steady_clock::now();
    size_t part = elem_num / thread_num; // �����, ������� ������� ����� ������� ������ 
    std::thread* thr = new std::thread[thread_num]; // ����� �������� �������, ����� ������ �������� �������
    for (int i = 0; i < thread_num - 1; i++)
    {
        thr[i] = thread(buf_mult_and_sum, first + i * part, second + i * part, part, std::ref(result[i]));
    }
    int adding1 = (thread_num - 1) * part;
    int adding2 = adding1 + elem_num % thread_num;
    thr[thread_num - 1] = thread(buf_mult_and_sum, first + adding1, second + adding2, part, std::ref(result[thread_num - 1]));
    auto end1 = std::chrono::steady_clock::now();
    auto elapsed_ms1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1);    
    for (int i = 0; i < thread_num; i++) {
        thr[i].join();
    }
    auto end2 = std::chrono::steady_clock::now();
    auto elapsed_ms2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - end1);
    buf_sum(result, thread_num, std::ref(total_result2));
    auto end3 = std::chrono::steady_clock::now();
    auto elapsed_ms3 = std::chrono::duration_cast<std::chrono::microseconds>(end3 - end2);
    std::cout << "The creating time: " << elapsed_ms1.count() << " microseconds\n";
    std::cout << "The join time: " << elapsed_ms2.count() << " microseconds\n";
    std::cout << "The summing time: " << elapsed_ms3.count() << " microseconds\n";
    std::cout << "Concurrent result: " << total_result2 << "\n";
    std::cout << "Diffrence between linear and concurrent results: " << total_result1 - total_result2 << "\n";
    auto elapsed_ms_total = std::chrono::duration_cast<std::chrono::microseconds>(end3 - begin1);
    auto ms_diffrence = elapsed_ms - elapsed_ms_total;
    std::cout << "Diffrence between linear and concurrent time: " << ms_diffrence.count() << " microseconds\n";
    return 0;
}
/// ���������� �������������
/// 100 000 000
/// 
/// ������ ������ ����������
/// 2.5 GB
/// 1 331 445 microseconds
/// 1.41026e+23
/// 
/// ������ ��������� ��� ��������
/// 1.6 GB
/// 579311 microseconds
/// 1.41026e+23
/// ��� 20 �������
/// 622238 microseconds
/// 20 ������� ���� ��������� ��������� �� 100 ���
/// 
/// Element number: 500 000 000
/// Thread number : 25
/// The time : 61 881 344 microseconds
/// Linear result : 1.76282e+25
/// The time : 21 941 536 microseconds
/// Concurrent result : 1.76282e+25
/// 8.2 GB -> ����������� ���� ��������
/// ��������� �������� ���-�� 2.5-3 ������
/// 
/// Element number: 400 000 000
/// Thread number : 40
/// The time : 36081546 microseconds
/// Linear result : 9.02564e+24
/// The time : 9540849 microseconds
/// Concurrent result : 9.02564e+24
/// 6.6 GB
/// 
/// Element number: 400000000
/// Thread number : 50
/// The time : 29747363 microseconds
/// Linear result : 9.02564e+24
/// The time : 8908595 microseconds
/// Concurrent result : 9.02564e+24
/// 6.6 GB
/// 
/// Element number: 300000000
/// Thread number : 30
/// The time : 13852544 microseconds
/// Linear result : 3.80769e+24
/// The time : 1851691 microseconds
/// Concurrent result : 3.80769e+24
/// 4.9 GB
/// ��������� 7.52
/// 
/// Element number: 400000000
/// Thread number : 30
/// The time : 30765999 microseconds
/// Linear result : 9.02564e+24
/// The time : 7324460 microseconds
/// Concurrent result : 9.02564e+24
/// 
/// Release:
/// Element number: 500000000
/// Thread number : 25
/// The time : 55634840 microseconds
/// Linear result : 1.76282e+25
/// The time : 23648030 microseconds
/// Concurrent result : 1.76282e+25