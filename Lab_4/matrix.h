#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <chrono>
#include "matrix_mul.cuh"

class Matrix {
public:
    int N;                      
    std::vector<float> data;   

    Matrix() : N(0) {}

    Matrix(int size) : N(size), data(size* size, 0.0f) {}


    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Не удалось открыть файл: " << filename << "\n";
            return false;
        }
        int rows, cols;
        file >> rows >> cols;
        if (rows != cols) {
            std::cerr << "Матрица должна быть квадратной\n";
            return false;
        }
        N = rows;
        data.resize(N * N);
        for (int i = 0; i < N * N; ++i) {
            file >> data[i];
        }
        return true;
    }

    void print() const {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                std::cout << data[i * N + j] << " ";
            }
            std::cout << "\n";
        }
    }

    Matrix cpu_dot(const Matrix& other) const {
        Matrix result(N);
        for (int row = 0; row < N; ++row) {
            for (int col = 0; col < N; ++col) {
                float sum = 0.0f;
                for (int k = 0; k < N; ++k) {
                    sum += data[row * N + k] * other.data[k * N + col];
                }
                result.data[row * N + col] = sum;
            }
        }
        return result;
    }


    Matrix gpu_dot(const Matrix& other, int blockSizeX = 16, int blockSizeY = 16) const {
        Matrix result(N);
        runCudaMul(data.data(), other.data.data(), result.data.data(), N, blockSizeX, blockSizeY);
        return result;
    }


    bool equals(const Matrix& other, float eps = 1e-2f) const {
        if (N != other.N) return false;
        for (int i = 0; i < N * N; ++i) {
            if (std::fabs(data[i] - other.data[i]) > eps) {
                return false;
            }
        }
        return true;
    }
};
