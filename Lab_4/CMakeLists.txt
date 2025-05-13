cmake_minimum_required(VERSION 3.10)
project(MatrixCUDA LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_STANDARD 14)

add_executable(MatrixCUDA main.cpp matrix_mul.cu)
