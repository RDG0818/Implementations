# Fits a linear equation y = mx + b to a dataset and targets
# Loss Function: L = 1/n * sum(Y - B @ X) ^ 2
# Grad Loss: grad = -2/n * sum(Y - B @ X) * X

from sklearn.datasets import load_diabetes
from sklearn.linear_model import LinearRegression
import pandas as pd
import numpy as np

diabetes = load_diabetes()
df = pd.DataFrame(diabetes.data, columns=diabetes.feature_names)
Y = np.array(diabetes.target)
X = np.concatenate((df.to_numpy(), np.ones((df.shape[0], 1))), axis=1)

# Least Squares Method (B = (X^T * X)^-1 * X^T * Y)
sol = np.linalg.inv(X.T @ X) @ X.T @ Y
print("Least Squares Coef:", sol[:-1])
print("Least Squares Intercept:", sol[-1], "\n")

# Gradient Descent Method:
theta = np.zeros((X.shape[1]))
prev = np.ones_like(theta)
alpha = 1.97 # manually tuned
epsilon = 1e-5
iterations = 0
while (np.linalg.norm(theta - prev) > epsilon):
    prev = theta.copy()
    theta -= alpha * 1/X.shape[0] * X.T @ (X @ theta - Y) # gradient of 1/n * sum((X @ theta - Y)^2) wrt theta
print("Gradient Descent Coef:", theta[:-1])
print("Gradient Descent Intercept:", theta[-1], "\n")

######## Solution ########
model = LinearRegression()
model.fit(df, Y)
coef = model.coef_
intercept = model.intercept_
print("Solution Coef:", coef)
print("Solution Intercept:", intercept)
