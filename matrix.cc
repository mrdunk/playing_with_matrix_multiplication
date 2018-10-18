#include "matrix.hpp"

Matrix::Matrix(size_t rows_, size_t columns_) {
  rows = rows_;
  columns = columns_;
  data.resize(columns * rows);
  data.clear();
}

int Matrix::get(const size_t r, const size_t c) const {
  assert(r < rows);
  assert(c < columns);
  return data[r * columns + c];
}

void Matrix::put(size_t r, size_t c, int value) {
  assert(r < rows);
  assert(c < columns);
  data[r * columns + c] = value;
}

Matrix Matrix::operator* (const Matrix& other) {
  assert(columns == other.rows);

  Matrix retrurnValue(rows, other.columns);
  for(size_t r = 0; r < rows; r++) {
    for(size_t c = 0; c < other.columns; c++) {
      int total = 0;
      for(size_t i = 0; i < columns; i++) {
        total += get(r, i) * other.get(i, c);
      }
      retrurnValue.put(r, c, total);
    }
  }

  return retrurnValue;
}

Matrix Matrix::operator+ (const Matrix& other) {
  assert(rows == other.rows);
  assert(columns == other.columns);

  Matrix retrurnValue(rows, columns);

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

void Matrix::display() {
  std::cout << rows << " x " << columns;
  for(size_t r = 0; r < rows; r++) {
    std::cout << "\n\t";
    for(size_t c = 0; c < columns; c++) {
      std::cout << get(r, c) << "\t";
    }
  }
  std::cout << std::endl;
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

