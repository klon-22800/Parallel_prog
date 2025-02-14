#include<iostream>
#include <chrono>
#include<matrix/matrix.h>

using namespace std;
using namespace matrix;

int main(){
    Matrix a;
    Matrix b;

    a.load("test_a.txt");
    b.load("test_a.txt");
    auto start = chrono::high_resolution_clock::now();
    Matrix res = a.dot(b);
    auto end = chrono::high_resolution_clock::now();
    res.print();
    cout << end - start;
    return 0;
}