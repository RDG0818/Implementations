# The power method and inverse power method are numerical algorithms that find a specified eigenvalue of a matrix. 
# This is most notably used in Google's PageRank algorithm, but is also used in PCA as well as finding the stationary distribution of a Markov Chain.

import numpy as np

def power_method(A, x, iter=101): # Finds the largest eigenvalue of A
    for _ in range(iter):
        y = A @ x
        muk = np.linalg.norm(y, np.inf)
        x = (1/muk) * y
    return muk

def inverse_power_method(A, x, q, iter=101): # Finds the eigenvalue closest to q of A
    for _ in range(iter):
        y = np.linalg.solve(A - q * np.eye(A.shape[0]), x)
        ynorm = np.linalg.norm(y, np.inf)
        x = y/ynorm
        muk = 1/ynorm + q
    return muk

def symmetric_power_method(A, x, iter=101):
    x /= np.linalg.norm(x, 2)
    for _ in range(iter):
        y = A @ x
        muk = y @ x
        x = y/np.linalg.norm(y, 2)
    return muk

def symmetric_inverse_power_method(A, x, q, iter=101):
    for _ in range(iter):
        y = np.linalg.solve(A - q * np.eye(A.shape[0]), x)
        lam = x.T @ y
        ynorm = np.linalg.norm(y, 2)
        x = y/ynorm
        muk = 1/lam + q
    return muk


