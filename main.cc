#include "matrix.hpp"
#include <chrono>
#include <thread>
#include <stdlib.h>

#define MAX_VALUE 100
#define MATRIX_SIZE 500
#define USE_THREADS true

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

  // Create a pair of fuckin great Matrix objects that we will be multiplying
  // together.
  Matrix big1(MATRIX_SIZE, MATRIX_SIZE);
  populateMatrix(big1);
  
  Matrix big2(MATRIX_SIZE, MATRIX_SIZE);
  populateMatrix(big2);
  
  startTimer();
  // Multiply the Matrix objects.
  Matrix result = big1 * big2;
  std::cout << returnTimerMs() << "ms" << std::endl;

  
  // Split the Matrix objects into 2 smaller Matrix objects each.
  Matrix big1a(MATRIX_SIZE / 2, MATRIX_SIZE);
  Matrix big1b(MATRIX_SIZE / 2, MATRIX_SIZE);
  splitMatrix(big1, big1a, big1b);

  Matrix big2a(MATRIX_SIZE, MATRIX_SIZE / 2);
  Matrix big2b(MATRIX_SIZE, MATRIX_SIZE / 2);
  splitMatrix(big2, big2a, big2b);

  // Multiply pairs of smaller Matrix objects together.
  auto multiplication =
    [=](Matrix& a, Matrix& b, Matrix& result) { result = a * b; };
  Matrix result2A(MATRIX_SIZE / 2, MATRIX_SIZE / 2);
  Matrix result2B(MATRIX_SIZE / 2, MATRIX_SIZE / 2);
  Matrix result2C(MATRIX_SIZE / 2, MATRIX_SIZE / 2);
  Matrix result2D(MATRIX_SIZE / 2, MATRIX_SIZE / 2);
  Matrix result2AB(MATRIX_SIZE / 2, MATRIX_SIZE);
  Matrix result2CD(MATRIX_SIZE / 2, MATRIX_SIZE);
  Matrix result2(MATRIX_SIZE, MATRIX_SIZE);

  startTimer();
  if(USE_THREADS) {
    std::thread t1(multiplication,
        std::ref(big1a), std::ref(big2a), std::ref(result2A));
    std::thread t2(multiplication,
        std::ref(big1a), std::ref(big2b), std::ref(result2B));
    std::thread t3(multiplication,
        std::ref(big1b), std::ref(big2a), std::ref(result2C));
    std::thread t4(multiplication,
        std::ref(big1b), std::ref(big2b), std::ref(result2D));
    t1.join();
    t2.join();
    t3.join();
    t4.join();
  } else {
    multiplication(big1a, big2a, result2A);
    multiplication(big1a, big2b, result2B);
    multiplication(big1b, big2a, result2C);
    multiplication(big1b, big2b, result2D);
  }
  
  // Join multiplied parts back together.
  if(USE_THREADS) {
    std::thread t5(concatenateMatrix,
        std::ref(result2A), std::ref(result2B), std::ref(result2AB));
    std::thread t6(concatenateMatrix,
        std::ref(result2C), std::ref(result2D), std::ref(result2CD));
    t5.join();
    t6.join();
  } else {
    concatenateMatrix(result2A, result2B, result2AB);
    concatenateMatrix(result2C, result2D, result2CD);
  }
  concatenateMatrix(result2AB, result2CD, result2);
  std::cout << returnTimerMs() << "ms" << std::endl;

  // Confirm both methods of multiplication have the same result.
  assert(result == result2);

  return 0;
}
