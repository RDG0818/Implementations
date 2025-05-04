import numpy as np
import sympy as sym
import matplotlib.pyplot as plt

x = sym.symbols('x')
p = sym.exp(x)
q = sym.exp(x)
f = (2 - x) * sym.exp(x) + x
u_exact = (x - 1) * (sym.exp(-x) - 1)

phi1 = x * (x - 1) ** 2
phi2 = x ** 3 * (x - 1)
phi3 = x ** 2 * (x - 1) ** 3
phi4 = x ** 3 * (x - 1) ** 2

def collocation(phi_list, pts):
    c = sym.symbols(f'c1:{len(phi_list)+1}')
    uh = sum(ci * ph for ci, ph in zip(c, phi_list))
    Pu = -sym.diff(p * sym.diff(uh, x), x) + q * uh
    A = sym.Matrix([[sym.expand(Pu.subs(x, pt)).coeff(ci) for ci in c] for pt in pts])
    b = sym.Matrix([f.subs(x, pt) for pt in pts])
    sol = A.LUsolve(b)
    uh_num = sym.expand(uh.subs(dict(zip(c, sol))))
    return sym.simplify(uh_num)

uh3 = collocation([phi1, phi2, phi3], [sym.Rational(1, 4), sym.Rational(1, 2), sym.Rational(3, 4)])
uh4 = collocation([phi1, phi2, phi3, phi4],
                  [sym.Rational(1, 5), sym.Rational(2, 5), sym.Rational(3, 5), sym.Rational(4, 5)])

grid = np.linspace(0, 1, 201)
u_e = sym.lambdify(x, u_exact, 'numpy')(grid)
u_h3 = sym.lambdify(x, uh3, 'numpy')(grid)
u_h4 = sym.lambdify(x, uh4, 'numpy')(grid)

print("max error 3-basis:", np.abs(u_e - u_h3).max())
print("max error 4-basis:", np.abs(u_e - u_h4).max())

plt.figure()
plt.plot(grid, u_e, label='Exact')
plt.plot(grid, u_h3, '--', label='Collocation 3φ', )
plt.plot(grid, u_h4, '--', label='Collocation 4φ')
plt.legend()
plt.xlabel('x')
plt.ylabel('u')
plt.title('Exact vs Collocation')
plt.show()
