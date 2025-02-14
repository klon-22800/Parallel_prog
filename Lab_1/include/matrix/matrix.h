#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iomanip>


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
			if (_cols != other._cols) {
				throw runtime_error("invalid sizes of matrix");
			}
			Matrix result(_rows, other._cols);
			for (int i = 0; i < _rows; ++i) {
				for (int j = 0; j < other._cols; ++j) {
					for (int k = 0; k < _cols; ++k) {
						result._data[i][j] += _data[i][k] * other._data[k][j];
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
