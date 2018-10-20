#include <iostream>
#include <thread>
#include <string.h>
#include <assert.h>

#include "matrix.hpp"

Matrix::Matrix(const size_t rows_,
               const size_t columns_,
               const MatrixOrientation orientation_) {
  rows = rows_;
  columns = columns_;
  orientation = orientation_;
  data.resize(columns * rows);
  data.clear();
}

int Matrix::get(const size_t r, const size_t c) const {
  assert(r < rows);
  assert(c < columns);
  if(orientation == horizontal) {
    //std::cout  << orientation << " " << (r * columns + c) << "\n";
    return data[r * columns + c];
  }
  //std::cout << orientation << " " << (r + c * rows) << "\n";
  return data[r + c * rows];
}

void Matrix::put(const size_t r, const size_t c, const int value) {
  assert(r < rows);
  assert(c < columns);
  if(orientation == horizontal) {
    data[r * columns + c] = value;
  } else {
    data[r + c * rows] = value;
  }
}

Matrix Matrix::operator* (const Matrix& other) {
  assert(columns == other.rows);
  assert(orientation == horizontal);
  assert(other.orientation == vertical);

  Matrix retrurnValue(rows, other.columns, horizontal);
  if(orientation == horizontal) {
    for(size_t r = 0; r < rows; r++) {
      for(size_t c = 0; c < other.columns; c++) {
        int total = 0;
        for(size_t i = 0; i < columns; i++) {
          total += get(r, i) * other.get(i, c);
        }
        retrurnValue.put(r, c, total);
      }
    }
  } else {
    for(size_t c = 0; c < other.columns; c++) {
      for(size_t r = 0; r < rows; r++) {
        int total = 0;
        for(size_t i = 0; i < columns; i++) {
          total += get(r, i) * other.get(i, c);
        }
        retrurnValue.put(r, c, total);
      }
    }
  }

  return retrurnValue;
}

Matrix Matrix::operator+ (const Matrix& other) {
  assert(rows == other.rows);
  assert(columns == other.columns);

  Matrix retrurnValue(rows, columns, horizontal);

  for(size_t r = 0; r < rows; r++) {
    for(size_t c = 0; c < other.columns; c++) {
      retrurnValue.put(r, c, get(r, c) + other.get(r, c));
    }
  }

  return retrurnValue;
}

bool Matrix::operator== (const Matrix& other) {
  if(rows != other.rows || columns != other.columns) {
    return false;
  }

  for(size_t r = 0; r < rows; r++) {
    for(size_t c = 0; c < columns; c++) {
      if(get(r, c) != other.get(r, c)) {
        return false;
      }
    }
  }
  return true;
}

bool Matrix::operator!= (const Matrix& other) {
  return !(*this == other);
}

void Matrix::display() const {
  std::cout << rows << " x " << columns << "  ";
  std::cout << (orientation==horizontal ? "H" : "V");
  for(size_t r = 0; r < rows; r++) {
    std::cout << "\n\t";
    for(size_t c = 0; c < columns; c++) {
      std::cout << get(r, c) << "\t";
    }
  }
  std::cout << std::endl;
}

bool Matrix::convertOrientation(const MatrixOrientation desiredOrientation) {
  if(desiredOrientation == orientation) {
    return false;
  }

  std::vector<int> tmpData = {};
  tmpData.resize(columns * rows);
  for(size_t r = 0; r < rows; r++) {
    for(size_t c = 0; c < columns; c++) {
      tmpData[r + (c * rows)] = data[(r * columns) + c];
    }
  }
  orientation = desiredOrientation;
  data = tmpData;

  return true;
}



void splitMatrix(Matrix& matrixIn,
                 Matrix& matrixOut1,
                 Matrix& matrixOut2) {
  size_t rows = matrixIn.rows;
  size_t columns = matrixIn.columns;

  if(rows > matrixOut1.rows) {
    for(size_t r = 0; r < rows / 2; r++) {
      for(size_t c = 0; c < columns; c++) {
        matrixOut1.put(r, c, matrixIn.get(r, c));
      }
    }
    for(size_t r = rows / 2; r < rows; r++) {
      for(size_t c = 0; c < columns; c++) {
        matrixOut2.put(r - rows / 2, c, matrixIn.get(r, c));
      }
    }
  } else {
    for(size_t r = 0; r < rows; r++) {
      for(size_t c = 0; c < columns / 2; c++) {
        matrixOut1.put(r, c, matrixIn.get(r, c));
      }
    }
    for(size_t r = 0; r < rows; r++) {
      for(size_t c = columns / 2; c < columns; c++) {
        matrixOut2.put(r, c - columns / 2, matrixIn.get(r, c));
      }
    }
  }
}

void concatenateMatrix(Matrix& matrixIn1,
                       Matrix& matrixIn2,
                       Matrix& result) {
  if(matrixIn1.columns + matrixIn2.columns == result.columns) {
    assert(matrixIn1.rows == matrixIn2.rows);
    assert(matrixIn1.rows == result.rows);
    for(size_t r = 0; r < matrixIn1.rows; r++) {
      for(size_t c = 0; c < matrixIn1.columns; c++) {
        result.put(r, c, matrixIn1.get(r, c));
      }
    }
    for(size_t r = 0; r < matrixIn1.rows; r++) {
      for(size_t c = matrixIn1.columns;
          c < matrixIn1.columns + matrixIn2.columns;
          c++) {
        result.put(r, c, matrixIn2.get(r, c - matrixIn1.columns));
      }
    }
  } else if(matrixIn1.rows + matrixIn2.rows == result.rows) {
    assert(matrixIn1.columns == matrixIn2.columns);
    assert(matrixIn1.columns == result.columns);
    for(size_t r = 0; r < matrixIn1.rows; r++) {
      for(size_t c = 0; c < matrixIn1.columns; c++) {
        result.put(r, c, matrixIn1.get(r, c));
      }
    }
    for(size_t r = matrixIn1.rows;
        r < matrixIn1.rows + matrixIn2.rows;
        r++) {
      for(size_t c = 0; c < matrixIn1.columns; c++) {
        result.put(r, c, matrixIn2.get(r - matrixIn1.rows, c));
      }
    }
  } else {
    assert(false);
  }
}

std::atomic<size_t> threadsInUse(0);

void splitThenMultiply(Matrix& matrixIn1,
                       Matrix& matrixIn2,
                       Matrix& result) {

  Matrix in1a(matrixIn1.rows / 2, matrixIn1.columns, matrixIn1.orientation);
  Matrix in1b(matrixIn1.rows / 2, matrixIn1.columns, matrixIn1.orientation);
  Matrix in2a(matrixIn2.rows, matrixIn2.columns / 2, matrixIn2.orientation);
  Matrix in2b(matrixIn2.rows, matrixIn2.columns / 2, matrixIn2.orientation);
  Matrix result1(matrixIn1.rows / 2, matrixIn2.columns / 2, horizontal);
  Matrix result2(matrixIn1.rows / 2, matrixIn2.columns / 2, horizontal);
  Matrix result3(matrixIn1.rows / 2, matrixIn2.columns / 2, horizontal);
  Matrix result4(matrixIn1.rows / 2, matrixIn2.columns / 2, horizontal);
  Matrix result12(matrixIn1.rows / 2, matrixIn2.columns, horizontal);
  Matrix result34(matrixIn1.rows / 2, matrixIn2.columns, horizontal);

  // Split the Matrix objects into 2 smaller Matrix objects each.
  splitMatrix(matrixIn1, in1a, in1b);
  splitMatrix(matrixIn2, in2a, in2b);

  if(USE_THREADS) {
    threadsInUse += 4;
    auto multiplication = [=](Matrix& a, Matrix& b, Matrix& result) {
        if(threadsInUse < MAX_THREADS && in1a.rows > SPLIT_TO_SIZE) {
          splitThenMultiply(a, b, result);
        } else {
          result = a * b; 
        }
      };

    // Multiply pairs of smaller Matrix objects together.
    std::thread t1(multiplication,
        std::ref(in1a), std::ref(in2a), std::ref(result1));
    std::thread t2(multiplication,
        std::ref(in1a), std::ref(in2b), std::ref(result2));
    std::thread t3(multiplication,
        std::ref(in1b), std::ref(in2a), std::ref(result3));
    std::thread t4(multiplication,
        std::ref(in1b), std::ref(in2b), std::ref(result4));
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    // Join results together.
    t1 = std::thread(concatenateMatrix,
        std::ref(result1),
        std::ref(result2),
        std::ref(result12));
    t2 = std::thread(concatenateMatrix,
        std::ref(result3),
        std::ref(result4),
        std::ref(result34));
    t1.join();
    t2.join();
    threadsInUse -= 4;
  } else {
    // Multiply pairs of smaller Matrix objects together.
    if(in1a.rows > SPLIT_TO_SIZE) {
      splitThenMultiply(in1a, in2a, result1);
      splitThenMultiply(in1a, in2b, result2);
      splitThenMultiply(in1b, in2a, result3);
      splitThenMultiply(in1b, in2b, result4);
    } else {
      result1 = in1a * in2a;
      result2 = in1a * in2b;
      result3 = in1b * in2a;
      result4 = in1b * in2b;
    }

    // Join results together.
    concatenateMatrix(result1, result2, result12);
    concatenateMatrix(result3, result4, result34);
  }
  concatenateMatrix(result12, result34, result);
}
