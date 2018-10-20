#ifndef PLAY_WITH_MATRIX__MATRIX_HPP
#define PLAY_WITH_MATRIX__MATRIX_HPP

#include <cstddef>
#include <vector>
#include <atomic>

#define USE_THREADS true
//#define USE_THREADS false
#define SPLIT_TO_SIZE 256
#define MAX_THREADS 8


extern std::atomic<size_t> threadsInUse;

enum MatrixOrientation {
  horizontal,
  vertical
};

class Matrix {
 public:
  size_t rows;
  size_t columns;
  MatrixOrientation orientation;
  // Container to hold the data.
  std::vector<int> data;

  Matrix(const size_t rows_,
         const size_t columns_,
         const MatrixOrientation orientation_);

  // Return a value at row, column.
  int get(const size_t r, const size_t c) const;

  // Set a value at row, column.
  void put(const size_t r, const size_t c, const int value);

  // Allow "matrix1 * matrix2" type syntax to multiply matrices.
  Matrix operator* (const Matrix& other);

  // Allow "matrix1 + matrix2" type syntax to add matrices.
  Matrix operator+ (const Matrix& other);

  // Allow "matrix1 == matrix2" type syntax to test 2 matrix's
  // for equality.
  // Returns:
  //   true if equal.
  //   false otherwise.
  bool operator== (const Matrix& other);

  // Allow "matrix1 != matrix2" type syntax to test 2 matrices
  // for inequality.
  // Returns:
  //   false if equal.
  //   true otherwise.
  bool operator!= (const Matrix& other);

  // Display Matrix content to stdout.
  void display() const;

  // Convert between horizontal and vertical rows in memory.
  bool convertOrientation(const MatrixOrientation desiredOrientation);
};


void splitMatrix(Matrix& matrixIn,
                 Matrix& matrixOut1,
                 Matrix& matrixOut2);

void concatenateMatrix(Matrix& matrixIn1,
                       Matrix& matrixIn2,
                       Matrix& result);

void splitThenMultiply(Matrix& matrixIn1,
                       Matrix& matrixIn2,
                       Matrix& result);

#endif  // PLAY_WITH_MATRIX__MATRIX_HPP
