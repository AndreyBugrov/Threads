# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.

import subprocess
import csv





# Press the green button in the gutter to run the script.
if __name__ == '__main__':

# tab / shift+tab


    to_prevent_deleting = input("If you purposely wanted to launch it, input anything")
    # process = subprocess.Popen(["D:\\PROJECTS\\Subprocess Scripts\\common_matrix_m=const.py"])
    # process1 = subprocess.Popen(["D:\\PROJECTS\\Subprocess Scripts\\linear_matrix_m=const.py"])
    # process2 = subprocess.Popen(["D:\\PROJECTS\\Subprocess Scripts\\common_matrix_n=const.py"])
    # process3 = subprocess.Popen(["D:\\PROJECTS\\Subprocess Scripts\\linear_matrix_n=const.py"])

    # process = subprocess.Popen(["D:\\PROJECTS\\Курсовая работа\\Threads\\Threads\\x64\\Release\\Threads.exe",
    #                                                       str_data_len, str_thread_num, "d"], stdout=subprocess.PIPE)
    for thread_num in [2,4,8]:
        str_thread_num = str(thread_num)
        name = "thread_train"+str_thread_num+".csv"
        with open(name, "w",newline="") as file:
            writer = csv.writer(file,delimiter=";")
            writer.writerow(('Sequential time',"Concurrent time",'Speedup','inaccuracy'))
            for data_len in range(100000000,250000001,10000000):
                str_data_len = str(data_len)
                process = subprocess.Popen(["D:\\PROJECTS\\Курсовая работа\\Threads\\Threads\\x64\\Release\\Threads.exe",
                                                          str_data_len, str_thread_num, "d"], stdout=subprocess.PIPE)
                data = process.communicate()
                lines = data[0].decode("UTF-8")
                out = lines.split(sep='\r\n')
                writer.writerow(out)


# See PyCharm help at https://www.jetbrains.com/help/pycharm/
