from cmath import tan
import matplotlib.pyplot as plt
import numpy as np


def Euler_method(interval: tuple, step: float):
    x = interval[0]
    y = interval[0]
    y_arr = []
    while x < interval[1]:
        y = y + step * drv(x, y)
        x = x + step
        y_arr.append(y)
        print(x, y)
    return y_arr

def Euler_Cauchy_method(interval: tuple, step: float):
    x = interval[0]
    y = interval[0]
    while x < interval[1]:
        y = y + step * drv(x, y)
        x = x + step
        print(x, y)
    return y

def Runghe_Kutta_method(interval: tuple, step: float):
    x = interval[0]
    y = interval[0]
    y_arr = []
    while x < interval[1]:
        k1 = drv(x, y)
        k2 = drv(x + step / 2, y + step * k1 / 2)
        k3 = drv(x + step / 2, y + step * k2 / 2)
        k4 = drv(x + step, y + step * k3)
        y = y + step * (k1 + 2 * k2 + 2 * k3 + k4) / 6
        x = x + step
        y_arr.append(y)
        print(x, y)
    return y_arr

def Multistep_Adam_method(drv, interval, step):
    pass

def drv(x: float, y: float):
    return (y+x)**2

def exact_func(interval: tuple, step: float):
    x = interval[0]
    y_arr = []
    while x < interval[1]:
        y = tan(x) - x
        x = x + step
        y_arr.append(y)
        print(x, y)
    return y_arr

if __name__ == "__main__":
    y_euler = Euler_method((0, 1), 0.1)
    y_euler_cauchy = Euler_Cauchy_method((0, 1), 0.1)
    y_runghe_Kutta = Runghe_Kutta_method((0, 1), 0.1)
    y_exact = exact_func((0, 1), 0.1)

    x = np.arange(0, 1, 0.1)

    print(x, y_euler)

    plt.scatter(x, y_euler, label="Euler")
    plt.scatter(x, y_euler_cauchy, label="Euler-Cauchy")
    plt.scatter(x, y_runghe_Kutta, label="Runghe-Kutta")
    plt.scatter(x, y_exact, label="Exact")
    plt.legend()
    plt.savefig("diffeq.png")