import pandas as pd
import matplotlib.pyplot as plt

def main():
    df = pd.read_csv("results.csv")

    grouped = df.groupby(["matrix_size", "block_size"])["gpu_time"].mean().reset_index()

    block_sizes = sorted(grouped["block_size"].unique())

    plt.figure(figsize=(10, 6))

    for block in block_sizes:
        subset = grouped[grouped["block_size"] == block]
        plt.plot(subset["matrix_size"], subset["gpu_time"], marker='o', label=f"Block size: {block}")

    plt.title("Зависимость GPU времени от размера матрицы")
    plt.xlabel("Размер матрицы (N x N)")
    plt.ylabel("Время выполнения (сек)")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    # plt.savefig("matrix_gpu_plot.png")  # сохранить в файл
    plt.show()  

if __name__ == "__main__":
    main()
