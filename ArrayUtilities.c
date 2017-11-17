#include "ArrayUtilities.h"
int factorial(int* ints, int start, int end) {
  int product = 1;
  for(int i = start; i <= end; i++) {
    product *= ints[i];
  }
  return product;
}

int sum(int* ints, int start, int end) {
  int sum = 0;
  for(int i = start; i <= end; i++) {
    sum += ints[i];
  }
  return sum;
}

int contiguousCountOf(int* ints, size_t size, int interest) {
  int count = 0;
  for(int i = 0; i < size; i++) {
    if(ints[i] == interest) {
      count += 1;
    }
    else {
      return count;
    }
  }
  return count;
}

int oneNonZero(int* ints, int* index, int* value, size_t size) {
  int seenZero = 0;
  for(int i = 0; i < size; i++) {
    if(ints[i] != 0)  {
      if(seenZero) {
        return 0;
      }
      else {
        seenZero = 1;
        *index = i;
        *value = ints[i];
      }
    }
  }
  return seenZero;
}

int lenBeforeZero(int* ints, int start, int end) {
  for(int i = start; i <=end; i++){
    if(ints[i] == 0) {
      return i;
    }
  }
  return 1 + end - start;
}

int lenBeforeNonZero(int* ints, int start, int end) {
  for(int i = start; i <= end; i++) {
    if(ints[i] != 0) {
      return i;
    }
  }
  return 1 + end - start;
}

void printArray(size_t size, int* arr, FILE* fp){
  fprintf(fp, "[");
  for(int i = 0; i < size; i++) {
    fprintf(fp, "%d,", arr[i]);
  }
  fprintf(fp, "]");
}
