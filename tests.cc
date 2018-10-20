#include <iostream>
#include <assert.h>

#include "matrix.hpp"

bool test_getAndPutAtDifferntOrientation() {
  bool returnVal = true;

  std::vector<int> data = {0, 1, 2, 3, 4, 5, 6};
  Matrix input1(2, 3, horizontal);
  Matrix input2(2, 3, vertical);

  input1.put(0, 0, data[0]);
  input2.put(0, 0, data[0]);

  input1.put(0, 1, data[1]);
  input2.put(0, 1, data[1]);

  input1.put(0, 2, data[2]);
  input2.put(0, 2, data[2]);

  input1.put(1, 0, data[3]);
  input2.put(1, 0, data[3]);

  input1.put(1, 1, data[4]);
  input2.put(1, 1, data[4]);

  input1.put(1, 2, data[5]);
  input2.put(1, 2, data[5]);

  returnVal &= (input1.get(0, 0) == input2.get(0, 0));
  returnVal &= (input1.get(0, 1) == input2.get(0, 1));
  returnVal &= (input1.get(0, 2) == input2.get(0, 2));
  returnVal &= (input1.get(1, 0) == input2.get(1, 0));
  returnVal &= (input1.get(1, 1) == input2.get(1, 1));
  returnVal &= (input1.get(1, 2) == input2.get(1, 2));

  returnVal &= (input1.data[1] != input2.data[1]);

  return returnVal;
}

bool test_covertOrientation() {
  bool returnVal = true;

  Matrix input1(2, 3, horizontal);
  input1.data = {0, 1, 2, 3, 4, 5};

  Matrix input2(2, 3, horizontal);
  input2.data = {0, 1, 2, 3, 4, 5};
  input2.convertOrientation(vertical);

  returnVal &= (input1 == input2);

  return returnVal;
}

bool test_MatrixMultiplication() {
  Matrix input1(2, 3, horizontal);
  input1.data = {0, 3, 5, 5, 5, 2};

  Matrix input2(3, 2, horizontal);
  input2.data = {3, 4, 3, -2, 4, -2};

  Matrix expected(2, 2, horizontal);
  expected.data = {29, -16, 38, 6};

  Matrix output = input1 * input2;

  return output == expected;
}

bool test_MatrixMultiplicationByUintVector() {
  Matrix input1(2, 3, horizontal);
  input1.data = {0, 3, 5, 5, 5, 2};

  Matrix unit(3, 3, horizontal);
  unit.data = {1, 0, 0, 0, 1, 0, 0, 0, 1};

  Matrix output = input1 * unit;

  return output == input1;
}

bool test_MatrixEqual() {
  Matrix input1(2, 3, horizontal);
  input1.data = {0, 3, 5, 5, 5, 2};

  return input1 == input1;
}

bool test_MatrixNotEqual() {
  Matrix input1(2, 3, horizontal);
  input1.data = {0, 3, 5, 5, 5, 2};

  Matrix input2(2, 3, horizontal);
  input2.data = {1, 3, 5, 5, 5, 2};

  return !(input1 == input2);
}


int main() {
  assert(test_getAndPutAtDifferntOrientation());
  assert(test_covertOrientation());
  assert(test_MatrixMultiplication());
  assert(test_MatrixMultiplicationByUintVector());
  assert(test_MatrixEqual());
  assert(test_MatrixNotEqual());

  std::cout << "All tests pass." << std::endl;

  return 0;
}

