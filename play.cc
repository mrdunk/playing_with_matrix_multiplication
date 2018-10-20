#include "matrix.hpp"

#include <iostream>
#include <chrono>

#define ARRAY_SIZE (1024 * 1024)
#define OPPERATION_COUNT 100000000
#define MATRIX_SIZE 1024
#define MATRIX_AREA (MATRIX_SIZE * MATRIX_SIZE)

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

int main() {
  std::vector<int> data;
  data.resize(ARRAY_SIZE);
  for(uint64_t skip = 1; skip < 1024; skip *= 2) {
    startTimer();
    for(uint64_t i = 0, j = 0; i < OPPERATION_COUNT; i++, j += skip) {
      data[j % ARRAY_SIZE] *= 3;
    }
    std::cout << skip << "\t" << returnTimerMs() << "ms" << std::endl;
  }

  std::cout << std::endl;

  Matrix matrix(MATRIX_SIZE, MATRIX_SIZE, vertical);
  for(uint64_t skip = 1; skip < 1024; skip *= 2) {
    startTimer();
    for(uint64_t i = 0, j = 0; i < OPPERATION_COUNT; i++, j += skip) {
      matrix.get(j % MATRIX_SIZE, (j % MATRIX_AREA) / MATRIX_SIZE);
    }
    std::cout << skip << "\t" << returnTimerMs() << "ms" << std::endl;
  }

}

