import subprocess
import csv
to_prevent_deleting = input("If you purposely wanted to launch it, input anything")
for thread_num in [2,4,8]:
    str_thread_num = str(thread_num)
    name = "new_vector_thread"+str_thread_num+".csv"
    with open(name, "w",newline="") as file:
        writer = csv.writer(file,delimiter=";")
        writer.writerow(('Последовательное время',"Параллельное время",
                         'Неточность','Ускорение'))
        for n in range(10000000,240000001,1000000):
            #'d' like for documentation
            # D:\PROJECTS\Курсовая работа\Threads\Threads\x64\Release
            process = subprocess.Popen(["D:\\PROJECTS\\Курсовая работа\\Threads\\Threads\\x64\\Release\\Threads.exe",
                                                      str(n), str_thread_num, 'd'], stdout=subprocess.PIPE)
            data = process.communicate()
            lines = data[0].decode("UTF-8")
            out = lines.split(sep='\r\n')
            writer.writerow(out)
            
