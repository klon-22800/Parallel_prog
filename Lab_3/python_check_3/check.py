import numpy as np
import subprocess
import sys
import os
import argparse

def read_matrix_from_file(file_path):
    with open(file_path, 'r') as f:
        lines = f.readlines()
        size = int(lines[0].strip())
        matrix = [list(map(float, line.strip().split())) for line in lines[1:size+1]]
    return np.array(matrix)

def generate_random_matrix(size):
    matrix = np.random.rand(size, size) * 10
    return np.round(matrix, 2)

def write_matrix_to_file(matrix, filename):
    with open(filename, 'w') as f:
        size = matrix.shape[0]
        f.write(f"{size}\n")
        for row in matrix:
            f.write(" ".join(map(str, row)) + "\n")

def parse_exe_output(output):
    lines = output.strip().split('\n')
    matrix_lines = []
    time_line = ""

    for line in lines:
        if "Elapsed time:" in line:
            time_line = line
        elif line.strip():
            matrix_lines.append(line)

    try:
        time_value = float(time_line.split(":")[1].split()[0])
    except (IndexError, ValueError):
        return None, None

    result_matrix = []
    for line in matrix_lines:
        try:
            result_matrix.append(list(map(float, line.strip().split())))
        except ValueError:
            continue

    return np.array(result_matrix), time_value

def run_mpi_test(matrix, num_processes, exe_path):
    test_file = "test_a.txt"
    write_matrix_to_file(matrix, test_file)

    expected = np.round(np.dot(matrix, matrix), 2)

    cmd = f"mpiexec -n {num_processes} {exe_path} {test_file}"
    result = subprocess.run(cmd, shell=True, capture_output=True, text=True)

    if result.returncode != 0:
        return None, "Execution failed", result.stderr

    actual_matrix, time_val = parse_exe_output(result.stdout)

    if actual_matrix is None or time_val is None:
        return None, "Output parsing failed", result.stdout

    if not np.allclose(expected, actual_matrix, atol=0.1):
        return None, "Matrix mismatch", result.stdout

    return time_val * 1000, None, None  

def main():
    parser = argparse.ArgumentParser()
    default_exe = os.path.join(os.path.dirname(__file__), "console.exe")
    parser.add_argument("--exe", type=str, default=default_exe, help="Path to compiled console executable")

    args = parser.parse_args()

    sizes = range(100, 2001, 100)  
    processes = [1, 4, 8, 12]
    repeats = 5

    with open("mpi_results.csv", "w") as f:
        f.write("Size,Processes,Time(ms)\n")

    for size in sizes:
        matrix = generate_random_matrix(size)
        for n_proc in processes:
            print(f"Testing size {size} with {n_proc} process(es)")
            times = []

            for i in range(repeats):
                time, error_type, error_log = run_mpi_test(matrix, n_proc, args.exe)

                if time is not None:
                    print(f"  Run {i+1}/{repeats}: {time:.2f} ms")
                    times.append(time)
                else:
                    print(f"  ❌ Run {i+1}/{repeats} failed: {error_type}")
                    with open("mpi_errors.log", "a") as log:
                        log.write(f"Size: {size}, Proc: {n_proc}, Error: {error_type}\n{error_log}\n\n")

            if times:
                avg = sum(times) / len(times)
                with open("mpi_results.csv", "a") as f:
                    f.write(f"{size},{n_proc},{avg:.2f}\n")
                print(f"✅ Avg time: {avg:.2f} ms")
            else:
                print("❌ All attempts failed.\n")

    print("\n🎯 Test complete. Results saved to mpi_results.csv")

if __name__ == "__main__":
    main()
