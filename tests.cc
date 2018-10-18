#include "matrix.hpp"

bool test_MatrixMultiplication() {
  Matrix input1(2, 3);
  input1.data = {0, 3, 5, 5, 5, 2};
  input1.display();

  Matrix input2(3, 2);
  input2.data = {3, 4, 3, -2, 4, -2};
  input2.display();

  Matrix expected(2, 2);
  expected.data = {29, -16, 38, 6};

  Matrix output = input1 * input2;
  output.display();

  return output == expected;
}

bool test_MatrixMultiplicationByUintVector() {
  Matrix input1(2, 3);
  input1.data = {0, 3, 5, 5, 5, 2};
  input1.display();

  Matrix unit(3, 3);
  unit.data = {1, 0, 0, 0, 1, 0, 0, 0, 1};
  unit.display();

  Matrix output = input1 * unit;
  output.display();

  return output == input1;
}

int main() {
  assert(test_MatrixMultiplication());
  assert(test_MatrixMultiplicationByUintVector());

  return 0;
}

