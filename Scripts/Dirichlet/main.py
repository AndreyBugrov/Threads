import subprocess
import csv

if __name__ == '__main__':
    # r or z in string + prevents deleting values
    gen_type = input("What kind of generator: rand (r) or zero (z)?")
    if gen_type == 'r':
        gen_type = 'rand'
    else:
        gen_type = 'zero'
    method_set = ('заглушка', 'seq', 'g-z_kill', 'fast', 'g-j_kill', 'wave', 'block')
    x_y = ("-200y+100", "200y-100", "-200x+100", "200x-100")
    for method in [1, 3, 5]:
        # FLOAT ARGUMENTS OF COMMAND LINE SHOULD BE SEPARATED
        for str_eps in ['0,1', '0,01', '0,001']:
            name = "dirichlet_" + method_set[method] + str_eps + "_" + gen_type + ".csv"
            with open(name, "w", newline="") as file:
                writer = csv.writer(file, delimiter=";")
                writer.writerow(('N', 'Итерации', 'Время', 'Погрешность'))
                for k_N in range(100, 501, 10):
                    print(method_set[method])
                    process = subprocess.Popen(["D:\\Projects\\Курсовая работа\\Threads\\Threads\\x64\\Release\\Dirichlet.exe",
                                                              str(k_N), str_eps, gen_type, str(method),
                                                x_y[0], x_y[1], x_y[2], x_y[3]], stdout=subprocess.PIPE)
                    data = process.communicate()
                    lines = data[0].decode("UTF-8")
                    out = lines.split(sep='\r\n')
                    writer.writerow(out)


# See PyCharm help at https://www.jetbrains.com/help/pycharm/
