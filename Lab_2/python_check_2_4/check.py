import numpy as np
import subprocess
import sys


def read_matrix_from_file(file_path):
    with open(file_path, 'r') as f:
        f.readline()
        matrix = [list(map(float, line.split())) for line in f.readlines()]
    return np.array(matrix)


def generate_random_matrix(rows, cols):
    matrix = np.random.rand(rows, cols) * 10
    return matrix


def write_matrix_to_file(matrix, filename):
    with open(filename, 'w') as f:
        rows, cols = matrix.shape
        f.write(f"{rows} {cols}\n") 
        for row in matrix:
            f.write(" ".join(map(str, row)) + "\n")


for i in range(10, 1000, 50):
    for k in range(10):
        rows, cols = i, i
        print(i)
        random_matrix = generate_random_matrix(rows, cols)
        write_matrix_to_file(random_matrix, "test_a.txt")
        matrix_a = read_matrix_from_file("test_a.txt")
        np_result = np.dot(matrix_a, matrix_a)
        np_result = np.round(np_result, 2)
        result = subprocess.run(["console.exe"], capture_output=True, text=True)
        matrix_lines = result.stdout.strip().split("\n")
        execution_time = matrix_lines[-1]
        matrix_lines = matrix_lines[:-1]
        matrix_from_exe = [list(map(float, line.split())) for line in matrix_lines]
        np_matrix_from_exe = np.array(matrix_from_exe)
        print(execution_time)

        # print("Результат умножения матриц (numpy):")
        # print(np_result)

        # print("\nРезультат из .exe:")
        # print(np_matrix_from_exe)

        # print("=========")
        # print("Время выполнения:", execution_time)

        print("======")
        if np.allclose(np_result, np_matrix_from_exe, rtol=1e-3):
            log = str(rows) + "\t" +str(execution_time[:-2]) +'\n'
            with open("log_omp_4.txt", 'a') as f:
                f.write(log)
        else:
            print("Ошибка в расчете")
            sys.exit(0)
