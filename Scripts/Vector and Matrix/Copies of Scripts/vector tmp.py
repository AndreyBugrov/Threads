import subprocess
import csv
to_prevent_deleting = input("If you purposely wanted to launch it, input anything")
thread_num =3
str_thread_num = str(thread_num)
name = "vector_tmp"+str_thread_num+".csv"
with open(name, "w",newline="") as file:
    writer = csv.writer(file,delimiter=";")
    writer.writerow(('Последовательное время',"Параллельное время",
                         'Неточность','Ускорение',
                         'Последовательный результат'))
    for n in range(10000000,241000000,10000000):
        #'d' like for documentation
        # D:\PROJECTS\Курсовая работа\Threads\Threads\x64\Release
        process = subprocess.Popen(["D:\\PROJECTS\\Курсовая работа\\Threads\\Threads\\x64\\Release\\Threads.exe",
                                                    str(n), str_thread_num, 'd'], stdout=subprocess.PIPE)
        data = process.communicate()
        lines = data[0].decode("UTF-8")
        out = lines.split(sep='\r\n')
        writer.writerow(out)
