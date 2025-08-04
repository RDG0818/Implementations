import numpy as np

def theta_march(v0, theta, mu, T, nt, f, x_grid):
    m = v0.size
    Dt = T / nt
    b1 = theta * mu
    b0 = 1 + 2 * b1
    B = np.zeros((m, m))
    for i in range(1, m - 1):
        B[i, i - 1] = -b1
        B[i, i] = b0
        B[i, i + 1] = -b1
    B[0, 0] = 1
    B[-1, -1] = 1
    f1 = (1 - theta) * mu
    f0 = 1 - 2 * f1
    v = v0.copy()
    for n in range(nt):
        t_current = n * Dt
        v_new = np.zeros_like(v)
        for j in range(1, m - 1):
            v_new[j] = f1 * v[j - 1] + f0 * v[j] + f1 * v[j + 1]
        v_new += Dt * f(x_grid, t_current + theta * Dt)
        v_new[0] = 0.0
        v_new[-1] = 0.0
        v = np.linalg.solve(B, v_new)
    return v

def main():
    a, b = 0.0, 1.0
    T = 1.0
    u_exact = lambda x, t: np.exp(t) * np.sin(np.pi * x)
    f_source = lambda x, t: (1 + np.pi**2) * np.exp(t) * np.sin(np.pi * x)
    u0 = lambda x: np.sin(np.pi * x)

    nx = 100
    nt_values = [10, 20, 40]
    theta_values = [0.5, 1.0]

    print(f"{'nx':<5} {'nt':<6} {'theta':<7} {'mu':<7} {'l∞ rel error':<15}")
    print("-" * 70)

    Dx = (b - a) / nx
    x_grid = np.linspace(a, b, nx + 1)
    v0 = u0(x_grid)
    uT_exact = u_exact(x_grid, T)

    for nt in nt_values:
        Dt = T / nt
        mu = Dt / Dx**2
        for theta in theta_values:
            vT_numerical = theta_march(v0, theta, mu, T, nt, f_source, x_grid)
            max_exact = np.max(np.abs(uT_exact))
            if max_exact == 0:
                rel_err = np.linalg.norm(vT_numerical, ord=np.inf)
            else:
                rel_err = np.linalg.norm(uT_exact - vT_numerical, ord=np.inf) / max_exact
            print(f"{nx:<5d} {nt:<6d} {theta:<7.2f} {mu:<7.2f} {rel_err:<15.6e}")

    print("-" * 70)

if __name__ == '__main__':
    main()
