#include <thread>
#include <vector>
#include <iostream>
#include <chrono>
#include <random>
using std::thread;
/// 1. Input from cmd
/// 2. Print difference between numbers
/// 3. Multiplying of matrixes and vectors
/// 

// for fast type changes
typedef long long long_type;
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
//void buf_mult_and_sum(long long* beginx, long long* beginy, size_t size, long long& result)
//{
//    for (int i = 0; i < size; i++) {
//        long long tmp = beginx[i] * beginy[i];
//        // tmp *= sin(beginx[i]) * sin(beginx[i]) + cos(beginx[i]) * cos(beginx[i]);
//        result += tmp;
//    }
//}

void get_elem_and_threads(int& elem_num, int& thread_num)
{
    std::cout << "Element number: ";
    std::cin >> elem_num;
    std::cout << "Thread number: ";
    std::cin >> thread_num;
}

int main()
{
   /* 
    auto begin = std::chrono::steady_clock::now();
    getchar();
    auto end = std::chrono::steady_clock::now();

    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    */

    int elem_num, thread_num;
    get_elem_and_threads(elem_num, thread_num);
    long_type* first = new long_type[elem_num]; // first vector
    long_type* second = new long_type[elem_num]; // second vector
    long_type* result = new long_type[thread_num]; // result of summing of every thread
    long_type total_result1 = 0.0; // is created by one thread
    long_type total_result2 = 0.0;
    for (int i = 0; i < elem_num; i++) {
        long_type element = long_type(i);
        if (i % 13 == 0) {
            element *= long_type(-1);
            second[i] = long_type(elem_num) + element;
        }
        else {
            second[i] = long_type(elem_num) - element;
        }
        first[i] = element;
    }
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
    size_t part = elem_num / thread_num; // часть, которая отдаётся почти каждому потоку 
    std::thread* thr = new std::thread[thread_num]; // После счётчика времени, чтобы учесть создание потоков
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

    //for (int k = 2; k < 26; k++) {
    //    thread_num = k;
    //    std::cout << "\nNumber of threads: " << k << "\n";
    //    for (int j = 0; j < 10; j++) {
    //        std::cout << "\n";
    //        total_result = 0.0;
    //        for (int i = 0; i < thread_num; i++) {
    //            result[i] = 0.0;
    //        }
    //        auto begin = std::chrono::steady_clock::now();
    //        buf_mult_and_sum(first, second, elem_num, std::ref(total_result));
    //        auto end = std::chrono::steady_clock::now();
    //        auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    //        std::cout << elapsed_ms.count() << "\n";
    //       // std::cout << "Linear result: " << total_result << "\n";
    //        //------------------------------------------------------------------------------------------------
    //        total_result = 0.0;
    //        auto begin1 = std::chrono::steady_clock::now();
    //        size_t part = elem_num / thread_num; // часть, которая отдаётся почти каждому потоку 
    //        std::thread* thr = new std::thread[thread_num]; // Чтобы учесть создание потоков
    //        for (int i = 0; i < thread_num - 1; i++)
    //        {
    //            thr[i] = thread(buf_mult_and_sum, first + i * part, second + i * part, part, std::ref(result[i]));
    //        }
    //        int adding = (thread_num - 1) + elem_num % thread_num;
    //        thr[thread_num - 1] = thread(buf_mult_and_sum, first + adding, second + adding, part, std::ref(result[thread_num - 1]));
    //        for (int i = 0; i < thread_num; i++) {
    //            thr[i].join();
    //        }
    //        buf_sum(result, thread_num, std::ref(total_result));
    //        auto end1 = std::chrono::steady_clock::now();
    //        auto elapsed_ms1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1);
    //        std::cout << elapsed_ms1.count() << "\n";
    //       // std::cout << "Concurrent result: " << total_result << "\n";
    //    }   
    //}
    return 0;
}
/// Результаты экспериментов
/// 100 000 000
/// 
/// Старая модель вычислений
/// 2.5 GB
/// 1 331 445 microseconds
/// 1.41026e+23
/// 
/// Теперь результат ещё страшнее
/// 1.6 GB
/// 579311 microseconds
/// 1.41026e+23
/// При 20 потоках
/// 622238 microseconds
/// 20 Потоков дают наилучший результат на 100 млн
/// 
/// Element number: 500 000 000
/// Thread number : 25
/// The time : 61 881 344 microseconds
/// Linear result : 1.76282e+25
/// The time : 21 941 536 microseconds
/// Concurrent result : 1.76282e+25
/// 8.2 GB -> Использован файл подкачки
/// Программа работала где-то 2.5-3 минуты
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
/// Отношение 7.52
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