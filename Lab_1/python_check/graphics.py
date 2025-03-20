import numpy as np
import matplotlib.pyplot as plt

def read_data_from_file(filename):
    x = []
    y = []
    with open(filename, 'r') as file:
        for line in file:
            data = line.split()
            x.append(int(data[0]))  
            y.append(int(data[1])) 
    return np.array(x), np.array(y)


filename = 'log.txt'  
x, y = read_data_from_file(filename)

plt.scatter(x, y, marker='o' , color='b', s = 10)  

plt.title('График логов')  
plt.xlabel('Размер матрицы N*N') 
plt.ylabel('Время выолнения в ns') 
plt.grid(True)  
plt.legend()  
plt.show()