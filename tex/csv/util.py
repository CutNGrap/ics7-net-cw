from time import process_time
import matplotlib.pyplot as plt
import pandas as pd

flag_rewrite_img = 1

def img_routine(plot, name):
    plot.legend()
    plot.grid()
    plt.ylabel("Затраченное время (мс)")
    plt.xlabel("Количество лучей (тыс. ед.)")

    if flag_rewrite_img:
        plt.savefig('../img/' + name + '.png')




def test():
    

    time = pd.read_csv('time.csv')
    print(time)

    

    fig = plt.figure(figsize=(10, 7))

    a = [x**2 / 1000 for x in range(300, 2100 + 1, 300)]
    plot = fig.add_subplot()
    plot.plot(a, time['0'],  label = "0 потоков", c = 'b')
    plot.plot(a, time['1'], ":", label="1 поток", c = 'g')
    plot.plot(a, time['2'], "--", label="2 потока", c = 'r')
    plot.plot(a, time['4'], "go-", label="4 потока", c = 'magenta')
    plot.plot(a, time['8'], "go:", label="8 потоков", c = 'black')
    plot.plot(a, time['16'], "go--", label="16 потоков", c = 'cyan')

    img_routine(plot, 'all_threads')
    


if __name__ == '__main__':
    test()
