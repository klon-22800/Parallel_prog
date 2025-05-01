import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def get_time(processes, size):
    match = df[(df['Processes'] == processes) & (df['Size'] == size)]['Time(ms)']
    return match.values[0] if not match.empty else np.nan

df = pd.read_csv("mpi_results.csv")

comparison = pd.DataFrame()
matrix_sizes = sorted(df['Size'].unique())
comparison['Size'] = matrix_sizes

comparison['1_process'] = [get_time(1, size) for size in matrix_sizes]
comparison['4_processes'] = [get_time(4, size) for size in matrix_sizes]
comparison['8_processes'] = [get_time(8, size) for size in matrix_sizes]  # <-- добавлено
comparison['12_processes'] = [get_time(12, size) for size in matrix_sizes]

# Ускорение
comparison['speedup_4vs1'] = comparison['1_process'] / comparison['4_processes']
comparison['speedup_8vs1'] = comparison['1_process'] / comparison['8_processes']  # <-- добавлено
comparison['speedup_12vs1'] = comparison['1_process'] / comparison['12_processes']

plt.figure(figsize=(16, 6))

# Время выполнения
plt.subplot(1, 2, 1)
plt.plot(comparison['Size'], comparison['1_process'], 'b-o', label='1 процесс')
plt.plot(comparison['Size'], comparison['4_processes'], 'g-o', label='4 процесса')
plt.plot(comparison['Size'], comparison['8_processes'], 'm-o', label='8 процессов')  # <-- добавлено
plt.plot(comparison['Size'], comparison['12_processes'], 'r-o', label='12 процессов')
plt.xlabel('Размер матрицы')
plt.ylabel('Время выполнения (мс)')
plt.title('Время выполнения для разного числа процессов')
plt.grid(True)
plt.legend()

# Ускорение
plt.subplot(1, 2, 2)
plt.plot(comparison['Size'], comparison['speedup_4vs1'], 'g--o', label='Ускорение 4 vs 1')
plt.plot(comparison['Size'], comparison['speedup_8vs1'], 'm--o', label='Ускорение 8 vs 1')  # <-- добавлено
plt.plot(comparison['Size'], comparison['speedup_12vs1'], 'r--o', label='Ускорение 12 vs 1')
plt.xlabel('Размер матрицы')
plt.ylabel('Ускорение (раз)')
plt.title('Ускорение выполнения относительно 1 процесса')
plt.grid(True)
plt.legend()

plt.tight_layout()
plt.show()

comparison.to_csv('performance_comparison.csv', index=False)
print("\nРезультаты сравнения сохранены в performance_comparison.csv")
