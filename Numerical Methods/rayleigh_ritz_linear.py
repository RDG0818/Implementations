import numpy as np
import matplotlib.pyplot as plt

def u_exact(x):
    return (x - 1) * (np.exp(-x) - 1)

def solve_rritz_linear(N):
    h = 1.0 / N
    x = np.linspace(0, 1, N + 1)

    n = N - 1
    A = np.zeros((n, n))
    b = np.zeros(n)

    for k in range(N):
        xL, xR = x[k], x[k + 1]

        Ie = 0.5 * h * (np.exp(xL) + np.exp(xR))

        dphi = np.array([-1 / h, 1 / h])

        Kgrad = np.outer(dphi, dphi) * Ie

        phi_L = np.array([1.0, 0.0])
        phi_R = np.array([0.0, 1.0])
        Mloc = 0.5 * h * (
            np.exp(xL) * np.outer(phi_L, phi_L) +
            np.exp(xR) * np.outer(phi_R, phi_R)
        )

        Ke = Kgrad + Mloc

        fL = (2 - xL) * np.exp(xL) + xL
        fR = (2 - xR) * np.exp(xR) + xR
        be = 0.5 * h * np.array([fL * phi_L[0] + fR * phi_R[0],
                                 fL * phi_L[1] + fR * phi_R[1]])

        for a in range(2):
            ia = k + a - 1
            if 0 <= ia < n:
                b[ia] += be[a]
                for b_ in range(2):
                    ib = k + b_ - 1
                    if 0 <= ib < n:
                        A[ia, ib] += Ke[a, b_]

    u_int = np.linalg.solve(A, b)
    u_num = np.hstack(([0.0], u_int, [0.0]))
    return x, u_num

if __name__ == "__main__":
    plt.figure(figsize=(10, 6))
    x_exact_plot = np.linspace(0, 1, 200)
    plt.plot(x_exact_plot, u_exact(x_exact_plot), 'k-', label='Exact Solution', linewidth=2)

    for N in (10, 20):
        x, u_h = solve_rritz_linear(N)
        err = np.abs(u_h - u_exact(x))

        print(f"\n=== N = {N} elements (h = {1/N:.3f}) ===")
        print(u_h) 
        print(f"max |error|   : {err.max():.6e}")
        print(f"discrete L2   : {np.linalg.norm(err)/np.sqrt(len(err)):.6e}")

        plt.plot(x, u_h, 'o--', label=f'Numerical N={N}', markersize=5)

    plt.xlabel('x')
    plt.ylabel('u(x)')
    plt.title('Comparison of Numerical and Exact Solutions')
    plt.legend()
    plt.grid(True)
    plt.show()