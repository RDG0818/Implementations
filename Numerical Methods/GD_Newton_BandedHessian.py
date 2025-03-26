"""
A comparison of gradient descent, Newton's method, and a Banded Hessian method on two unconstrained minimization problems.
"""

import numpy as np
from sympy import symbols, diff, Matrix
import time

np.set_printoptions(precision=10)
def gradient(f,Vars):
    """Symbolic Computation of Grad."""
    return Matrix([diff(f,var) for var in Vars])

def grad_Hessian(f,Vars):
    """Symbolic Computation of Grad and Hessian."""
    G = Matrix([diff(f,var) for var in Vars])
    H = Matrix([[diff(G[i],var) for var in Vars] for i in range(len(Vars))])
    return G, H

def evaluate(Math_Form,Vars,P):
    """Evaluation of Mathematical Expression at P."""
    return np.array(Math_Form.subs( zip(Vars,P) )).astype(float)
# list(zip(Vars,P)) = [(x0,P[0]), (x1,P[1]), (x2,P[2])]

def compare(fx, Vars, sol, gdalpha, ngamma, qngamma, itmax, epsilon, round_num):
    G,H = grad_Hessian(fx,Vars)

    banded_H = np.zeros_like(H)
    for i in range(len(Vars)):
        for j in range(len(Vars)):
                if abs(i - j) <= 1:
                    banded_H[i, j] = H[i, j]
    banded_H = Matrix(banded_H)

    print("Gradient:", G)
    print("Hessian:", H)
    print("Banded Hessian:", banded_H)

    #--------------------------------------------------
    print("Find the minimum, by the Gradient Descent method")
    #--------------------------------------------------

    start_time = time.time()
    iterations = 0

    x = np.zeros((len(Vars),)); print("x_0 =",x)
    for k in range(1, itmax+1):
        iterations += 1
        x -= gdalpha * evaluate(G, Vars, x).flatten()
        if np.linalg.norm(sol - x) < epsilon:
            break
    
    print(f"x_{iterations-1} = {np.round(x, round_num)}")

    end_time = time.time()
    print(f"Elapsed time: {1000 * (end_time - start_time):.4f} milliseconds\n")
    #--------------------------------------------------
    print("Find the minimum, by the Newton's method")
    #--------------------------------------------------

    start_time = time.time()
    iterations = 0

    x = np.zeros((len(Vars),)); print("x_0 =",x)
    for k in range(1,itmax+1):
        iterations += 1
        grad = evaluate(G,Vars,x); hess = evaluate(H,Vars,x)
        x = x - ngamma * np.squeeze(np.linalg.inv(hess) @ grad)
        if np.linalg.norm(sol - x) < epsilon:
            break

    print(f"x_{iterations-1} = {np.round(x, round_num)}")  
    
    end_time = time.time()
    print(f"Elapsed time: {1000 * (end_time - start_time):.4f} milliseconds\n")
    #--------------------------------------------------
    print("Find the minimum, by the Quasi-Newton's method")
    #--------------------------------------------------
    
    start_time = time.time()
    iterations = 0

    x = np.zeros((len(Vars),)); print("x_0 =",x)
    for k in range(1,itmax+1):
        iterations += 1
        grad = evaluate(G,Vars,x); hess = evaluate(banded_H,Vars,x)
        x = x - qngamma * np.squeeze(np.linalg.inv(hess) @ grad)
        if np.linalg.norm(sol - x) < epsilon:
            break

    print(f"x_{iterations-1} = {np.round(x, round_num)}") 

    end_time = time.time()
    print(f"Elapsed time: {1000 * (end_time - start_time):.4f} milliseconds\n")

if __name__ == '__main__':
    x0, x1, x2, x3 = Vars4 = symbols('x:4')
    x_0, x_1, x_2, x_3, x_4, x_5 = Vars6 = symbols('x:6')
    f4 = (10 * (x0 - 2) ** 2 + 10 * (x1 - 3) ** 2 
          + 10 * (x2 - 4) ** 2 + 10 * (x3 - 5) ** 2 
          + (x0 - 2) * (x3 - 5))
    f6 = (20 * (x_0 - 10) ** 2 + 20 * (x_1 - 11) ** 2 
          + 20 * (x_2 - 12) ** 2 + 20 * (x_3 - 13) ** 2 
          + 20 * (x_4 - 14) ** 2 + 20 * (x_5 - 15) ** 2 
          + (x_0 - 10) * (x_5 - 15))
    print("Function of 4 Variables:\n")
    compare(f4, Vars4, np.array([2, 3, 4, 5]), 0.005, 1, 1, 1000, 1e-5, 5)
    print("\nFunction of 6 Variables:\n")
    compare(f6, Vars6, np.array([10, 11, 12, 13, 14, 15]), 0.0005, 1, 1, 1000, 1e-5, 5)