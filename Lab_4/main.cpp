#include "matrix.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {


    int blockSize = std::atoi(argv[1]);
    std::string inputFile = (argc >= 3) ? argv[2] : "test.txt";

    std::cout << blockSize << "\n";

    Matrix A;
    auto start = std::chrono::high_resolution_clock::now();

    if (!A.loadFromFile(inputFile)) {
        return 1;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Read time: "
        << std::chrono::duration<double>(end - start).count()
        << " s\n";
    std::cout << "Init matrix size: " <<A.N <<" \n" ;


    //A.print();

    //// CPU умножение для проверки 
    //auto start_cpu = std::chrono::high_resolution_clock::now();
    //Matrix C_cpu = A.cpu_dot(A);
    //auto end_cpu = std::chrono::high_resolution_clock::now();

    //std::cout << "\nCPU res:\n";
    ////C_cpu.print();
    //std::cout << "CPU time: "
    //    << std::chrono::duration<double>(end_cpu - start_cpu).count()
    //    << " s\n";

    // GPU умножение
    start = std::chrono::high_resolution_clock::now();
    Matrix C_gpu = A.gpu_dot(A, blockSize, blockSize);
    end = std::chrono::high_resolution_clock::now();

    //C_gpu.print();

    std::cout << "GPU time: "
        << std::chrono::duration<double>(end - start).count()
        << " s\n";

    //проверка резов
    //std::cout << "\nCorrect? " 
    //    << (C_cpu.equals(C_gpu) ? "yes" : "no") << "\n";

    return 0;
}
