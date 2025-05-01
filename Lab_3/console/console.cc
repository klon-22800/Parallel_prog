#include "matrix.h"
#include <mpi.h>
#include <iostream>
#include <chrono>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (argc < 2) {
        if (world_rank == 0)
            std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        MPI_Finalize();
        return 1;
    }

    std::string filename = argv[1];

    Matrix mat;
    if (world_rank == 0) {
        mat = Matrix::from_file(filename);
    }

    MPI_Barrier(MPI_COMM_WORLD); 
    auto start = std::chrono::high_resolution_clock::now();

    Matrix result = mat.multiply_parallel(mat);

    MPI_Barrier(MPI_COMM_WORLD);
    auto end = std::chrono::high_resolution_clock::now();

    if (world_rank == 0) {
        result.print();
        std::chrono::duration<double> diff = end - start;
        std::cout << "Elapsed time: " << diff.count() << " seconds\n";
    }

    MPI_Finalize();
    return 0;
}
