import subprocess

# Examples:
"""
1.
 # if you want to get only return code in terminal or get also output in PyCharm
 code = subprocess.call(["D:\\Algorithms_and_Complexity_Analysis\\x64\\Release\\Algorithms.exe",
                                str(i),str((i**2)//10),str(1),str(1000000),str(0),str(2)])
        print(code)
2.
Подробнее о subprocess.run см https://pythonru.com/osnovy/python-3-subprocess

process = subprocess.run(args, *, stdin=None, input=None, stdout=None,
stderr=None, capture _output=False, shell=False,
cwd=None, timeout=None, check=False,
encoding=None, errors=None, text=None,
env=None, universal newlines=None)

Аргументы функции, за исключением обязательного args - являются не обязательны и следовательно задаются только ключевыми словами.

args - запускаемая программа с аргументами,
stdin=None - поток данных, отправляемых в процесс,
input=None - поток данных, отправляемых в процесс,
stdout=None - поток вывода программы,
stderr=None - поток ошибок программы,
capture_output=False - захват вывода stdout и stderr,
shell=False - используйте True, если программа и ее аргументы представлены как одна строка,
cwd=None - путь к рабочему каталогу запускаемой программы,
timeout=None - максимально допустимое время выполнения запущенной программы в секундах,
check=False - вызывает исключение при завершении запущенной программы с ошибками,
encoding=None - кодировка, если input - строка,
errors=None - обработчик ошибок кодировки,
text=None - текстовый режим для stdin, stdout и stderr,
env=None - переменные среды для нового процесса,
universal_newlines=None - то же, что и параметр text.

3.
    get all the information
process = subprocess.Popen(["D:\\Курсовая работа\\Threads\\Threads\\x64\\Release\\Threads.exe",
                                            str_data_len, str_thread_num, "d"], stdout=subprocess.PIPE)
                data = process.communicate()
                lines = data[0].decode("UTF-8")
"""
