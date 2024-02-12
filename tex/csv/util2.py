from time import process_time
import matplotlib.pyplot as plt
import pandas as pd

flag_rewrite_img = 1

def img_routine(plot, name):
    plot.legend()
    plot.grid()
    plt.ylabel("Затраченное время (мс)")
    plt.xlabel("Количество потоков (ед.)")

    if flag_rewrite_img:
        plt.savefig('../img/' + name + '.png')




def test():
    

    no_m = pd.read_csv('time.csv')
    print(no_m)
    b = [no_m["rays"][i] **2 // 1000 for i in range(7)]
    print(b)

    

    name = ["threads_1800"]
    df = [no_m]
    col = [5]
    for i in range(1):
        fig = plt.figure(figsize=(10, 7))

        a = [0, 1, 2, 4, 8, 16, 32, 64]
        b = [no_m[str(j)][col[i]] for j in a]
        plot = fig.add_subplot()
        plot.plot(a, b, c = 'b')

        img_routine(plot, name[i])
    


if __name__ == '__main__':
    test()