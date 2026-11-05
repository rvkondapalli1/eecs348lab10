#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

class Matrix {
private:
    int size_;
    std::unique_ptr<int[]> data_;

    int index(int row, int col) const {
        if (row < 0 || row >= size_ || col < 0 || col >= size_) {
            throw std::out_of_range("Matrix index is out of bounds");
        }
        return row * size_ + col;
    }

public:
    explicit Matrix(int size)
        : size_(size), data_(new int[size * size]()) {
        if (size <= 0) {
            throw std::invalid_argument("Matrix size must be positive");
        }
    }

    // Make an independent copy so passing a Matrix by value is safe.
    Matrix(const Matrix& other) : Matrix(other.size_) {
        for (int i = 0; i < size_ * size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            Matrix copy(other);
            std::swap(size_, copy.size_);
            data_.swap(copy.data_);
        }
        return *this;
    }

    int size() const { return size_; }

    int get(int row, int col) const { return data_[index(row, col)]; }

    void set(int row, int col, int value) {
        data_[index(row, col)] = value;
    }

    void read(std::istream& input) {
        for (int row = 0; row < size_; ++row) {
            for (int col = 0; col < size_; ++col) {
                int value;
                if (!(input >> value)) {
                    throw std::runtime_error("Not enough matrix values in the file");
                }
                set(row, col, value);
            }
        }
    }

    void print() const {
        for (int row = 0; row < size_; ++row) {
            for (int col = 0; col < size_; ++col) {
                std::cout << std::setw(4) << get(row, col);
            }
            std::cout << '\n';
        }
    }

    Matrix operator+(const Matrix& other) const {
        if (size_ != other.size_) {
            throw std::invalid_argument("Cannot add matrices of different sizes");
        }
        Matrix result(size_);
        for (int row = 0; row < size_; ++row) {
            for (int col = 0; col < size_; ++col) {
                result.set(row, col, get(row, col) + other.get(row, col));
            }
        }
        return result;
    }

    Matrix operator*(const Matrix& other) const {
        if (size_ != other.size_) {
            throw std::invalid_argument("Cannot multiply matrices of different sizes");
        }
        Matrix result(size_);
        for (int row = 0; row < size_; ++row) {
            for (int col = 0; col < size_; ++col) {
                int sum = 0;
                for (int k = 0; k < size_; ++k) {
                    sum += get(row, k) * other.get(k, col);
                }
                result.set(row, col, sum);
            }
        }
        return result;
    }

    int mainDiagonalSum() const {
        int sum = 0;
        for (int i = 0; i < size_; ++i) {
            sum += get(i, i);
        }
        return sum;
    }

    int secondaryDiagonalSum() const {
        int sum = 0;
        for (int i = 0; i < size_; ++i) {
            sum += get(i, size_ - 1 - i);
        }
        return sum;
    }
};

// These operations receive a copy and never change the original matrix.
Matrix swapRows(Matrix matrix, int first = 0, int second = 1) {
    if (first < 0 || second < 0 || first >= matrix.size() ||
        second >= matrix.size()) {
        throw std::out_of_range("Invalid row index");
    }
    for (int col = 0; col < matrix.size(); ++col) {
        int temp = matrix.get(first, col);
        matrix.set(first, col, matrix.get(second, col));
        matrix.set(second, col, temp);
    }
    return matrix;
}

Matrix swapColumns(Matrix matrix, int first = 0, int second = 1) {
    if (first < 0 || second < 0 || first >= matrix.size() ||
        second >= matrix.size()) {
        throw std::out_of_range("Invalid column index");
    }
    for (int row = 0; row < matrix.size(); ++row) {
        int temp = matrix.get(row, first);
        matrix.set(row, first, matrix.get(row, second));
        matrix.set(row, second, temp);
    }
    return matrix;
}

Matrix updateElement(Matrix matrix, int row = 0, int col = 0,
                     int value = 100) {
    matrix.set(row, col, value);
    return matrix;
}

int main() {
    std::cout << "Enter input filename: " << std::endl;
    std::string filename;
    std::getline(std::cin, filename);

    std::ifstream input(filename.c_str());
    if (!input) {
        std::cerr << "Error: Cannot open input file: " << filename << '\n';
        return 1;
    }

    int n;
    if (!(input >> n) || n <= 0) {
        std::cerr << "Error: Invalid matrix size\n";
        return 1;
    }

    try {
        Matrix A(n);
        Matrix B(n);
        A.read(input);
        B.read(input);

        std::cout << "Matrix A:\n";
        A.print();
        std::cout << "\nMatrix B:\n";
        B.print();

        std::cout << "\nA + B:\n";
        (A + B).print();

        std::cout << "\nA * B:\n";
        (A * B).print();

        std::cout << "\nDiagonal sums for Matrix A:\n";
        std::cout << "Main diagonal sum: " << A.mainDiagonalSum() << '\n';
        std::cout << "Secondary diagonal sum: " << A.secondaryDiagonalSum() << '\n';

        // Demonstrate custom indices, as shown in the provided sample output.
        // swapRows(A), swapColumns(A), and updateElement(A) use defaults.
        if (n > 2) {
            std::cout << "\nProblem 5 - Rows 0 and 2 swapped:\n";
            swapRows(A, 0, 2).print();

            std::cout << "\nProblem 6 - Columns 0 and 2 swapped:\n";
            swapColumns(A, 0, 2).print();

            std::cout << "\nProblem 7 - Updated matrix:\n";
            updateElement(A, 1, 2, 99).print();
        } else {
            std::cout << "\nProblem 5 - Rows swapped:\n";
            if (n > 1) swapRows(A).print();
            else A.print();

            std::cout << "\nProblem 6 - Columns swapped:\n";
            if (n > 1) swapColumns(A).print();
            else A.print();

            std::cout << "\nProblem 7 - Updated matrix:\n";
            updateElement(A).print();
        }
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
