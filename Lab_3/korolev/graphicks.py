import matplotlib.pyplot as plt

sizes = [x for x in range(500, 4001, 500)]
mean_4 = [84.6, 1930.4, 6738.1, 16304.2, 33096.7, 59445.7, 96883.5, 145453.66 ]
mean_12 = [52.4785, 731.785, 2355.84, 5761.95, 11671.6, 21385.8, 35684.5, 56968.3]
mean_20 = [22.0014, 125.598, 431.894, 1121.58, 3146.51, 6670.65, 11407.5, 17614]


plt.figure(figsize=(12, 5))

plt.subplot(1, 3, 1)
plt.plot(sizes, mean_4, marker='o', label='4 потока')
plt.plot(sizes, mean_12, marker='o', label='12 потоков')
plt.plot(sizes, mean_20, marker='o', label='20 потоков')
plt.title('Время умножения матриц')
plt.xlabel('Размер матрицы (NxN)')
plt.ylabel('Время (мс)')
plt.legend()
plt.grid(True)

speedup_4_12 = [m4 / m12  for m4, m12 in zip(mean_4, mean_12)]
speedup_4_20 = [m4 / m20  for m4, m20 in zip(mean_4, mean_20)]

plt.subplot(1, 3, 2)
plt.plot(sizes, speedup_4_12, marker='o', color='green')

plt.title('Ускорение %: 4 потока / 12 потоков')
plt.xlabel('Размер матрицы (NxN)')
plt.ylabel('Кратность ускорения')
plt.grid(True)

plt.subplot(1, 3, 3)
plt.plot(sizes, speedup_4_20, marker='o', color='red')
plt.title('Ускорение %: 4 потока / 20 потоков')
plt.xlabel('Размер матрицы (NxN)')
plt.ylabel('Кратность ускорения')
plt.grid(True)

plt.tight_layout()
plt.show()