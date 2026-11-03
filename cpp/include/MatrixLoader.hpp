#ifndef MATRIX_LOADER_HPP
#define MATRIX_LOADER_HPP

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class MatrixLoader {
public:
	static std::vector<std::vector<double>> loadCSV(const std::string& filename, int N = 20) {
		std::vector<std::vector<double>> matrix(N, std::vector<double>(N, 0.0));
		std::ifstream file(filename);

		if (!file.is_open()) {
			std::cerr << "Error: Could not open file " << filename << std::endl;
			return matrix;
		}

		std::string line;
		int row = 0;
		while (std::getline(file, line) && row < N) {
			std::stringstream ss(line);
			std::string val;
			int col = 0;
			while (std::getline(ss, val, ',') && col < N) {
				matrix[row][col] = std::stod(val);
				col++;
			}
			row++;
		}

		std::cout << "Successfully loaded wavefront matrix from " << filename << std::endl;
		return matrix;
	}
};

#endif
