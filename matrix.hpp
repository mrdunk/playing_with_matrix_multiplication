#ifndef PLAY_WITH_MATRIX__MATRIX_HPP
#define PLAY_WITH_MATRIX__MATRIX_HPP

#include <iostream>
#include <cstddef>
#include <vector>
#include <assert.h>

class Matrix {
 public:
  size_t rows;
  size_t columns;
  // Container to hold the data.
  std::vector<int> data;

  Matrix(size_t rows_, size_t columns_);

  // Return a value at row, column.
  int get(const size_t r, const size_t c) const;

  // Set a value at row, column.
  void put(size_t r, size_t c, int value);

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
  void display();
};


void splitMatrix(Matrix& matrixIn,
                 Matrix& matrixOut1,
                 Matrix& matrixOut2);

void concatenateMatrix(Matrix& matrixIn1,
                       Matrix& matrixIn2,
                       Matrix& result);

#endif  // PLAY_WITH_MATRIX__MATRIX_HPP
