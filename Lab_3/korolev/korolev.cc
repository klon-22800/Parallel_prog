#include <mpi.h>

#include "matrix.cc"
#include <iostream>
#include <chrono>
#include <vector>
#include <numeric>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    std::vector<int> sizes = {};
    for (int size = 500; size <= 4000; size += 500) {
        sizes.push_back(size);
    }
    
    for (int size : sizes) {
        std::vector<double> timings;

        if (world_rank == 0) {
            std::cout << size << "*" << size << "  " << world_size << " thread\n";
        }

        for (int run = 0; run < 2; ++run) {
            Matrix mat;
            if (world_rank == 0) {
                mat = Matrix::generate(size, size);
                std::cout << "Generate" << std::endl;
            }

            if (world_size > 1) MPI_Barrier(MPI_COMM_WORLD);

            auto start = std::chrono::high_resolution_clock::now();
            Matrix result = mat.multiply_parallel(mat);
            if (world_size > 1) MPI_Barrier(MPI_COMM_WORLD);
            auto end = std::chrono::high_resolution_clock::now();

            if (world_rank == 0) {
                std::chrono::duration<double, std::milli> duration = end - start;
                timings.push_back(duration.count());
                std::cout << duration.count() << " ms\n";
            }
        }

        if (world_rank == 0) {
            double mean = std::accumulate(timings.begin(), timings.end(), 0.0) / timings.size();
            std::cout << "mean : " << mean << " ms\n\n";
        }
    }

    MPI_Finalize();
    return 0;
}
