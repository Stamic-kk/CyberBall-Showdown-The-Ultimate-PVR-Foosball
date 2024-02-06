import numpy as np
import matplotlib.pyplot as plt

X_MAX = 320
Y_MAX = 240
origin = np.array([X_MAX/2, Y_MAX/2])

def generate_bias():
    return np.random.normal(0, 0.25, 2)

observation = np.array([origin], dtype=object)
for i in range(50):
    bias = generate_bias()
    point = origin + ((i**.25)**2, i) + generate_bias()
    # print(point)
    observation = np.append(observation, [point], axis=0)
plt.figure()
plt.scatter(observation[:,0], observation[:,1])
plt.show()
