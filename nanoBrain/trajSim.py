import numpy as np
import matplotlib.pyplot as plt
import sys


X_MAX = 320
Y_MAX = 240
origin = np.array([X_MAX/2, Y_MAX/2])

def generate_bias():
    return np.random.normal(0, 0.25, 2)

observation = np.array([origin], dtype=object)
for i in range(0,150,5):
    bias = generate_bias()
    point = origin + ((i**.25), i*2) + generate_bias()
    # print(point)
    observation = np.append(observation, [point], axis=0)
    
if sys.argv[1] == "show":
    plt.figure()
    plt.scatter(observation[:,0], observation[:,1])
    plt.show()
   
elif sys.argv[1] == "save":
    file = open("inputs/traj.txt", "w")
    file.write(str(observation)[1:-2])

else:
    print(observation)
