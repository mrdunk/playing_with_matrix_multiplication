#include "matrix.hpp"

#include <chrono>
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_VALUE 100
#define MAX_MATRIX_SIZE (1024 * 4)
#define ROW_DIRECTION_A horizontal
//#define ROW_DIRECTION_A vertical
//#define ROW_DIRECTION_B horizontal
#define ROW_DIRECTION_B vertical
#define RUN_COUNT 100000

std::chrono::time_point<std::chrono::steady_clock> start;
void startTimer() {
  start = std::chrono::steady_clock::now();
}

double returnTimerMs() {
  std::chrono::time_point<std::chrono::steady_clock> end =
    std::chrono::steady_clock::now();
  std::chrono::microseconds diff =
    std::chrono::duration_cast<std::chrono::microseconds>
    (end - start);
  return (double)diff.count() / 1000;
}

// Populate the Matrix passed in with random values.
void populateMatrix(Matrix& matrix) {
  size_t rows = matrix.rows;
  size_t columns = matrix.columns;
  for(size_t r = 0; r < rows; r++) {
    for(size_t c = 0; c < columns; c++) {
      matrix.put(r, c, rand() % MAX_VALUE);
    }
  }
}

int main() {
  printf("Matrix size   no of runs        Time whole        Time split        "
         "Time diff\n");

  for(int matrixSize = 2; matrixSize <= MAX_MATRIX_SIZE; matrixSize *= 2) {
    // Create a pair of fuckin great Matrix objects that we will be multiplying
    // together.
    Matrix big1(matrixSize, matrixSize, ROW_DIRECTION_A);
    populateMatrix(big1);

    Matrix big2(matrixSize, matrixSize, ROW_DIRECTION_B);
    populateMatrix(big2);

    Matrix result(0, 0, horizontal); 
    startTimer();
    // Multiply the Matrix objects.
    for(size_t i = 0; i <= RUN_COUNT / (matrixSize * matrixSize); i++) {
      result = big1 * big2;
    }
    double time1 = returnTimerMs();


    Matrix result2(matrixSize, matrixSize, horizontal);

    startTimer();
    for(size_t i = 0; i <= RUN_COUNT / (matrixSize * matrixSize); i++) {
      splitThenMultiply(big1, big2, result2);
    }
    double time2 = returnTimerMs();

    double timeDiff = time1 - time2;
    double timeAverage = (time1 + time2) / 2;
    printf("%8d %15d %15.3fms %15.3fms %15.1f%%\n",
           matrixSize,
           (RUN_COUNT / (matrixSize * matrixSize) +1),
           time1,
           time2,
           (100 * timeDiff / timeAverage));

    // Confirm both methods of multiplication have the same result.
    assert(result == result2);
  }

  return 0;
}
