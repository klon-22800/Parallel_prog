#pragma once
#include <mpi.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>

class Matrix {
public:
    std::vector<std::vector<float>> data;

    Matrix() = default;

    Matrix(int rows, int cols) {
        data.resize(rows, std::vector<float>(cols, 0));
    }

    int rows() const {
        return static_cast<int>(data.size());
    }

    int cols() const {
        return data.empty() ? 0 : static_cast<int>(data[0].size());
    }

    Matrix from_file(const std::string& filename) {
        std::ifstream in(filename);
        if (!in) {
            throw std::runtime_error("Cannot open file: " + filename);
        }

        int size;
        in >> size;

        Matrix mat(size, size);
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                in >> mat.data[i][j];
            }
        }
        return mat;
    }

    Matrix generate(int rows, int cols) {
        Matrix res(rows, cols);
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(1, 10);

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                res.data[i][j] = dis(gen);
            }
        }
        return res;

    }


    Matrix multiply_parallel(const Matrix& other) const {
        int world_size, world_rank;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

        int N = rows();
        if (cols() != other.rows()) {
            if (world_rank == 0)
                std::cerr << "Matrix size mismatch" << std::endl;
            return {};
        }

        MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

        Matrix b_matrix(N, N);
        if (world_rank == 0) {
            b_matrix = other;
        }
        for (int i = 0; i < N; ++i) {
            MPI_Bcast(b_matrix.data[i].data(), N, MPI_FLOAT, 0, MPI_COMM_WORLD);
        }

        int rows_per_proc = N / world_size;
        int remainder = N % world_size;

        int my_rows = rows_per_proc + (world_rank < remainder ? 1 : 0);
        Matrix local_a(my_rows, N);

        std::vector<int> send_counts(world_size);
        std::vector<int> displs(world_size);
        int offset = 0;
        for (int i = 0; i < world_size; ++i) {
            send_counts[i] = rows_per_proc + (i < remainder ? 1 : 0);
            displs[i] = offset;
            offset += send_counts[i];
        }

        if (world_rank == 0) {
            for (int i = 1; i < world_size; ++i) {
                for (int r = 0; r < send_counts[i]; ++r) {
                    MPI_Send(data[displs[i] + r].data(), N, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
                }
            }
            for (int r = 0; r < my_rows; ++r) {
                local_a.data[r] = data[r];
            }
        }
        else {
            for (int r = 0; r < my_rows; ++r) {
                MPI_Recv(local_a.data[r].data(), N, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        Matrix local_result(my_rows, N);
        for (int i = 0; i < my_rows; ++i) {
            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < N; ++k) {
                    local_result.data[i][j] += local_a.data[i][k] * b_matrix.data[k][j];
                }
            }
        }

        Matrix result;
        if (world_rank == 0) {
            result = Matrix(N, N);
            for (int r = 0; r < my_rows; ++r) {
                result.data[r] = local_result.data[r];
            }
            for (int i = 1; i < world_size; ++i) {
                for (int r = 0; r < send_counts[i]; ++r) {
                    MPI_Recv(result.data[displs[i] + r].data(), N, MPI_FLOAT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
            }
        }
        else {
            for (int r = 0; r < my_rows; ++r) {
                MPI_Send(local_result.data[r].data(), N, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
            }
        }

        return result;
    }


    void  print() const {
        for (const auto& row : data) {
            for (float val : row) {
                std::cout << val << ' ';
            }
            std::cout << '\n';
        }
    }

};
