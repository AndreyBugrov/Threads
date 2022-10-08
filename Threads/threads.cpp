#include <thread>
#include <vector>
#include <iostream>
#include <chrono>
#include <random>
using std::thread;
/// <summary>
/// Переменное количество элементов векторов 
/// Переменное количество потоков 
/// Сравнение времени с последовательным фрагментом
/// Сравнение ответов с последовательным фрагментом
/// </summary>
/// 

// Multiplying pairs of values and writing the results to an array
void buf_mult(double* beginx,  double* beginy, size_t size, double* result)
{
    for (int i = 0; i < size; i++) {
        result[i] = beginx[i] * beginy[i];
    }
}
// Merging all buf elements into one sum (into result value)
void buf_sum(double* buf, size_t size, double& result) 
{
    for (int i = 0; i < size; i++) {
        result += buf[i];
    }
}

void buf_mult_and_sum(double* beginx, double* beginy, size_t size, double& result) 
{
    for (int i = 0; i < size; i++) {
        double tmp = beginx[i] * beginy[i];
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
    double* first = new double[elem_num]; // первый вектор
    double* second = new double[elem_num]; // второй вектор
   // double* preresult = new double[elem_num]; // вектор результатов умножения
    double* result = new double[thread_num]; // результат сложения после каждого потока
    double total_result = 0; // конечный результат, который создаётся одним потоком
    for (int i = 0; i < elem_num; i++) {
        double element = double(i);
        if (i % 13 == 0) {
            element *= -1.0;
            second[i] = double(elem_num) + element;
        }
        else {
            second[i] = double(elem_num) - element;
        }
        first[i] = element;
    }
    for (int i = 0; i < thread_num; i++) {
        result[i] = 0.0;
    }
    auto begin = std::chrono::steady_clock::now();
    buf_mult_and_sum(first, second, elem_num, std::ref(total_result));
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    std::cout << "The time: " << elapsed_ms.count() << " microseconds\n";
    std::cout << "Linear result: " << total_result << "\n";

    //------------------------------------------------------------------------------------------------

    total_result = 0.0;
    auto begin1 = std::chrono::steady_clock::now();
    size_t part = elem_num / thread_num; // часть, которая отдаётся почти каждому потоку 
    std::thread* thr = new std::thread[thread_num]; // Чтобы учесть создание потоков
    for (int i = 0; i < thread_num - 1; i++)
    {
        thr[i] = thread(buf_mult_and_sum, first + i * part, second + i * part, part, std::ref(result[i]));
    }
    int adding = (thread_num - 1) + elem_num % thread_num;
    thr[thread_num - 1] = thread(buf_mult_and_sum, first + adding, second + adding, part, std::ref(result[thread_num - 1]));
    for (int i = 0; i < thread_num; i++) {
        thr[i].join();
    }
    buf_sum(result, thread_num, std::ref(total_result));
    auto end1 = std::chrono::steady_clock::now();
    auto elapsed_ms1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1);
    std::cout << "The time: " << elapsed_ms1.count() << " microseconds\n";
    std::cout << "Concurrent result: " << total_result << "\n";

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
/// А смысл тогда жить (зачёркнуто) параллелить...
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