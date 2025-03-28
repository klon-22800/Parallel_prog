#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iomanip>
#include <omp.h>


using namespace std; 

namespace matrix {
	class Matrix {
	private:
		vector<vector<float>> _data;
		unsigned int _rows;
		unsigned int _cols;
	public:
		Matrix() {
			_rows = 0;
			_cols = 0;
			_data.resize(_rows, vector<float>(_cols, 0.0));
		};

		Matrix(unsigned int rows, unsigned int cols) {
			_rows = rows;
			_cols = cols;
			_data.resize(_rows, vector<float>(_cols, 0.0));
		}

		void load(const string& path) {
			std::ifstream in(path);
			if (in.is_open()) {
				in >> _rows >> _cols;
				Matrix res(_rows, _cols);

				for (int i = 0; i < _rows; ++i) {
					for (int j = 0; j < _cols; ++j) {
						in >> res._data[i][j];
					}
				}
				this->_data = res._data;
			}
			else {
				throw runtime_error("error open file");
			}
		}

		void save(const string& path) {
			ofstream out(path);
			if (out.is_open()) {
				out << _rows << " " << _cols << endl;
				for (const auto& row : _data) {
					for (float elem : row) {
						out << elem << " "; 
					}
					out << endl;
				}
			}
			else {
				throw runtime_error("error open file");
			}

			out.close();
		}

		Matrix dot(const Matrix& other) const {
			if (_cols != other._rows) {
				throw runtime_error("invalid sizes of matrix");
			}
			Matrix result(_rows, other._cols);



			#pragma omp parallel num_threads(4)
			{
				#pragma omp for
				for (int i = 0; i < _rows; ++i) {
					for (int j = 0; j < other._cols; ++j) {
						float sum = 0.0;
						#pragma omp simd reduction(+:sum)
						for (int k = 0; k < _cols; ++k) {
							sum += _data[i][k] * other._data[k][j];
						}
						result._data[i][j] = sum;
					}
				}
			}

			

			return result;
		}



		void print() const {
			cout << fixed << setprecision(2);
			for (const auto& row : _data) {
				for (float elem : row) {
					cout << elem << " ";
				}
				cout << endl;
			}
		}
	};
};
