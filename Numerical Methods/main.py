import numpy as np

A = np.array([[2, 1, -2], [0, 3, -1], [4, 5, 1]])
temp = np.dot(np.matrix_transpose(A), A)
print(np.linalg.eigvals(temp))
