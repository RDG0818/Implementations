import numpy as np

col1 = [0,0,0,1,1,0,1,0,0,0]
col2 = [0,0,0,0,0,1,0,1,0,1]
col3 = [0,0,0,1,0,0,1,1,1,1]
col4 = [1,0,0,0,0,0,1,0,1,0]
col5 = [0,1,1,0,0,0,0,0,1,0]
A = np.array([col1, col2, col3, col4, col5]).T
q = np.array([0,0,0,0,0,0,0,1,1,1])
U, S, V = np.linalg.svd(A, full_matrices=False)
for k in range(1, 6):
    Uk = U[:, :k]
    Sk = np.diag(S[:k])
    Vk = V[:k, :]
    qhat = Uk.T @ q
    Dk = Sk @ Vk
    cosines = []
    for j in range(5):
        Dkej = Dk[:, j]
        cosines.append((np.dot(qhat, Dkej))/(np.linalg.norm(qhat) * np.linalg.norm(Dkej)))
    print(np.array(cosines))