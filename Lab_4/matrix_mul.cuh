#pragma once

void runCudaMul(const float* h_A, const float* h_B, float* h_C, int N,
    int blockSizeX = 16, int blockSizeY = 16);
