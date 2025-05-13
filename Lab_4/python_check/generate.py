import random
import subprocess
import re
import os
import csv

def generate_matrix(size, filename):
    with open(filename, 'w') as f:
        f.write(f"{size} {size}\n")
        for _ in range(size):
            row = [f"{random.uniform(-10, 10):.2f}" for _ in range(size)]
            f.write(" ".join(row) + "\n")

def run_once(exe, block, filename):
    try:
        result = subprocess.run([exe, str(block), filename], capture_output=True, text=True)
        match = re.search(r"GPU time:\s+([\d.]+)", result.stdout)
        if match:
            return float(match.group(1))
    except Exception as e:
        print(f"Ошибка запуска: {e}")
    return None

def main():
    exe_path = "MatrixCUDA.exe"
    matrix_file = "test_matrix.txt"
    csv_file = "results.csv"


    with open(csv_file, mode="w", newline='') as f:
        writer = csv.writer(f)
        writer.writerow(["matrix_size", "block_size", "run", "gpu_time"])

        for block_size in [4, 8, 16, 32]:
            for matrix_size in range(1000, 5001, 1000):
                print(f"\n=== Тест: матрица {matrix_size}x{matrix_size}, блок {block_size}x{block_size} ===")
                for i in range(5):  
                    print(f"  Повтор {i+1}/5...")
                    generate_matrix(matrix_size, matrix_file)
                    t = run_once(exe_path, block_size, matrix_file)

                    if t is not None:
                        print(f"GPU time: {t:.6f} s")
                        writer.writerow([matrix_size, block_size, i + 1, t])
                    else:
                        print(f"ошибка выполнения")

                if os.path.exists(matrix_file):
                    os.remove(matrix_file)

    print(f"\n Данные сохранены в {csv_file}")

if __name__ == "__main__":
    main()
