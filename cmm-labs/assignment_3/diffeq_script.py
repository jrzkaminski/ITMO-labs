def Euler_method(interval: tuple, step: float):
    x = interval[0]
    y = interval[0]
    while x < interval[1]:
        y = y + step * drv(x, y)
        x = x + step
        print(x, y)
    return y

def Euler_Cauchy_method(drv, interval, step):
    x = interval[0]
    y = interval[1]
    while x < interval[2]:
        y = y + step * drv(x, y)
        x = x + step
        print(x, y)
    return y

def Runghe_Kutta_method(drv, interval, step):
    x = interval[0]
    y = interval[1]
    while x < interval[2]:
        k1 = drv(x, y)
        k2 = drv(x + step / 2, y + step * k1 / 2)
        k3 = drv(x + step / 2, y + step * k2 / 2)
        k4 = drv(x + step, y + step * k3)
        y = y + step * (k1 + 2 * k2 + 2 * k3 + k4) / 6
        x = x + step
        print(x, y)
    return y

def Multistep_Adam_method(drv, interval, step):
    pass

def drv(x: float, y: float):
    return (y+x)**2

Euler_method((0, 0.5), 0.1)