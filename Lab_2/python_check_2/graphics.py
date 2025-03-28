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


def mean_update(x, y):
    new_x = []
    new_y = []

    for i in range(2, 21):
        new_x.append(x[i*10-1])
        new_y.append(np.mean(y[(i-1)*10:i*10]))

    return np.array(new_x), np.array(new_y)

filename_1 = 'Lab_2\\python_check_2\\log.txt'  
filename_2 = 'Lab_2\\python_check_2\\log_omp_4.txt'  

 
x, y = read_data_from_file(filename_2)

# plt.scatter(x, y, marker='o' , color='b', s = 10)  

# plt.title('График логов')  
# plt.xlabel('Размер матрицы N*N') 
# plt.ylabel('Время выолнения в ns') 
# plt.grid(True)  
# plt.legend()  
# plt.show()

x, y = read_data_from_file(filename_1)
x_omp, y_omp = read_data_from_file(filename_2)

x, y = mean_update(x, y)
x_omp, y_omp = mean_update(x_omp, y_omp)

delta = y/y_omp 



plt.plot(x, delta)  

plt.title('Сравнение однопоточного алгоритма с OMP(4 потока)')  
plt.xlabel('Размер матрицы N*N') 
plt.ylabel('Разница') 
plt.grid(True)  
plt.legend()  
plt.show()