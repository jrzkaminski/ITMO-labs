from cmath import tan
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd


def Euler_method(interval: tuple, step: float):
    x = interval[0]
    y = interval[0]
    y_arr = []
    while x < interval[1]:
        y = y + step * drv(x, y)
        x = x + step
        y_arr.append(y)
    return y_arr


def Euler_Cauchy_method(interval: tuple, step: float):
    x = interval[0]
    y = interval[0]
    y_arr = []
    while x < interval[1]:
        y = y + (step * drv(x, y) + step * drv(x + step, y + step * drv(x, y))) / 2
        x = x + step
        y_arr.append(y)
    return y_arr


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
    return y_arr


def Multistep_Adams_method(interval: tuple, step: float):
    x = interval[0] + step * 5
    y_arr = Runghe_Kutta_method((0, 0.5), 0.1)
    print(y_arr)
    y = y_arr[-1]
    while x < interval[1]:
        y = y + step * (55 * drv(x, y) - 59 * drv(x - step, y - step * drv(x, y)) + 37 * drv(x - 2 * step,
                        y - 2 * step * drv(x, y)) - 9 * drv(x - 3 * step, y - 3 * step * drv(x, y))) / 24
        x = x + step
        y_arr.append(y)
    print(y_arr)
    return y_arr


def drv(x: float, y: float):
    return (y+x)**2


def exact_func(interval: tuple, step: float):
    x = interval[0]
    y_arr = []
    while x < interval[1]:
        y = tan(x) - x
        x = x + step
        y_arr.append(y.real)
    return y_arr


if __name__ == "__main__":
    y_euler = Euler_method((0, 1), 0.1)
    y_euler_cauchy = Euler_Cauchy_method((0, 1), 0.1)
    y_runghe_Kutta = Runghe_Kutta_method((0, 1), 0.1)
    y_multistep_adam = Multistep_Adams_method((0, 1), 0.1)
    y_exact = exact_func((0.1, 1.1), 0.1)

    x = np.arange(0, 1.01, 0.1)

    df = pd.DataFrame({'x': x, 'Euler': y_euler, 'Euler-Cauchy': y_euler_cauchy,
                      'Runghe-Kutta': y_runghe_Kutta, 'Multistep-Adam': y_multistep_adam, 'Exact': y_exact})

    print(df)

    plt.plot(x, y_euler, label="Euler")
    plt.plot(x, y_euler_cauchy, label="Euler-Cauchy")
    plt.plot(x, y_runghe_Kutta, label="Runghe-Kutta")
    plt.plot(x, y_multistep_adam, label="Multistep-Adam")
    plt.plot(x, y_exact, label="Exact")
    plt.legend()
    plt.grid()
    plt.savefig("diffeq.png")
