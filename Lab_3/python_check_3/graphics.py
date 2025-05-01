import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("mpi_results.csv")
for proc in df['Processes'].unique():
    subset = df[df['Processes'] == proc]
    plt.plot(subset['Size'], subset['Time(ms)'], label=f'{proc} процессов')

plt.xlabel('Размер матрицы')
plt.ylabel('Время (ms)')
plt.title('Производительность MPI умножения матриц')
plt.legend()
plt.grid()
plt.show()
