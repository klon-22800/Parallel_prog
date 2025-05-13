#include <cuda_runtime.h>
#include "matrix_mul.cuh"

__global__ void matrixMulCUDA(const float* A, const float* B, float* C, int N) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < N && col < N) {
        float sum = 0.0f;
        for (int k = 0; k < N; ++k) {
            sum += A[row * N + k] * B[k * N + col];
        }
        C[row * N + col] = sum;
    }
}

void runCudaMul(const float* h_A, const float* h_B, float* h_C, int N,
    int blockSizeX, int blockSizeY) {
    size_t size = N * N * sizeof(float);
    float* d_A, * d_B, * d_C;

    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);

    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    dim3 block(blockSizeX, blockSizeY);
    dim3 grid((N + block.x - 1) / block.x, (N + block.y - 1) / block.y);
    matrixMulCUDA << <grid, block >> > (d_A, d_B, d_C, N);
    cudaDeviceSynchronize();

    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
}

