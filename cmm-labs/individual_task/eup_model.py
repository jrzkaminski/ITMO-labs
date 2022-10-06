from scipy.optimize import minimize, brute
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import os
currentdir = os.getcwd()


data = pd.read_csv(currentdir+'/data/poland_eup.csv')
data.dropna(inplace=True)
data = data.sort_values(by='year', ascending=True)
data = data.reset_index(drop=True)
data


def derivatives(y, params):
    E, U, P = y
    lam_u_e, lam_p_u, lam_e_u, lam_e_p, lam_u_p, d_e, d_u = params
    dEdt = lam_u_e * U - lam_e_u * E - lam_e_p * E + d_e
    dUdt = lam_p_u * P + lam_e_u * E - lam_u_p * U - lam_u_e * U + d_u
    dPdt = lam_e_p * E + lam_u_p * U - lam_p_u * P
    return dEdt, dUdt, dPdt


def rk4(y, params, t, h):
    E_pred = []
    U_pred = []
    P_pred = []

    for i in range(len(t)):
        k1 = np.array(derivatives(y, params))
        k2 = np.array(derivatives(y + h * k1 / 2, params))
        k3 = np.array(derivatives(y + h * k2 / 2, params))
        k4 = np.array(derivatives(y + h * k3, params))
        y = y + h * (k1 + 2 * k2 + 2 * k3 + k4) / 6
        E_pred.append(y[0])
        U_pred.append(y[1])
        P_pred.append(y[2])

    y_pred = E_pred, U_pred, P_pred

    return y_pred


def mean_absolute_percentage_error(params, E, U, P):
    E_pred, U_pred, P_pred = rk4(y0, params, t, 1)
    return np.mean(np.abs((E - E_pred) / E)) + np.mean(np.abs((U - U_pred) / U)) + np.mean(np.abs((P - P_pred) / P))


if __name__ == '__main__':

    # Initial guess
    params = (6.51826215e-02,  1.17101395e-03, -4.89964539e-03, -2.24443190e-04,
              1.69655460e-03,  4.16971862e-03,  1.07993439e+02)

    # grid of years
    t = np.linspace(2008, 2022, 15)

    # Initial vector
    E0, U0, P0 = data['employed, thousands'].iloc[0], data['unemployed, thousands'].iloc[0], data['prisoners, thousands'].iloc[0]
    y0 = E0, U0, P0

    # Real values
    y_true = (data['employed, thousands'],
              data['unemployed, thousands'], data['prisoners, thousands'])

    # Apply optimization for parameters
    if params == []:
        res_brute = brute(mean_absolute_percentage_error, ((0, 1), (0, 1), (0, 1), (
            0, 1), (0, 1), (0, 1000), (0, 100)), args=((y_true)), Ns=10, full_output=True)
        params = res_brute[0]

    # Apply optimization with initial guess
    res = minimize(mean_absolute_percentage_error, params,
                   args=((y_true)), bounds=((0, 1), (0, 1), (0, 1), (0, 1), (0, 1), (0, 1000), (0, 100)), method='SLSQP', tol=1e-6)

    # Make forecast

    E_pred, U_pred, P_pred = rk4(y0, res.x, t, 1)

    # Plot results
    plt.scatter(data['year'], data['unemployed, thousands'],
                label='unemployed actual')
    plt.xlabel('year')
    plt.ylabel('unemployed (age 15-89), thousands')
    plt.title('Unemployed people in Poland (2008-2022)')
    plt.plot(t, U_pred, color='red', label='unemployed predicted')
    plt.legend()
    plt.savefig("unemployed.png")
    plt.show()

    plt.scatter(data['year'], data['prisoners, thousands'],
                label='prisoners actual')
    plt.title('Imprisoned people in Poland (2008-2022)')
    plt.xlabel('year')
    plt.ylabel('prisoners, thousands')
    plt.plot(t, P_pred, color='orange', label='prisoners predicted')
    plt.legend()
    plt.savefig("prisoners.png")
    plt.show()

    plt.scatter(data['year'], data['employed, thousands'],
                label='employed actual')
    plt.title('Employed people in Poland (2008-2022)')
    plt.xlabel('year')
    plt.ylabel('employed (age 15-89), thousands')
    plt.plot(t, E_pred, color='green', label='employed predicted')
    plt.legend()
    plt.savefig("employed.png")
    plt.show()
