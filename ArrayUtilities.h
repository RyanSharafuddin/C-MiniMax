#ifndef ARRAY_UTILITIES_H
#define ARRAY_UTILITIES_H
#include<stdlib.h>
#include<stdio.h>

//computes the factorial from start to end; includes end
int factorial(int* ints, int start, int end);
int sum(int* ints, int start, int end);
/* Given an array ints and a size, returns true
 * if there is exactly one nonzero value in the array
 * and puts the index of that value into index and the
 * value into value. At the end, index and value are the
 * index and value of the first nonzero element of the array, or
 * whatever they were before if there are no nonzero elements.
 */
int oneNonZero(int* ints, int* index, int* value, size_t size);
//returns index of first zero, or 1+end-start if there are no zeros
int lenBeforeZero(int* ints, int start, int end);
int lenBeforeNonZero(int* ints, int start, int end);
//returns number of interest in array
int contiguousCountOf(int* ints, size_t size, int interest);
void printArray(size_t size, int* arr, FILE* fp);



#endif
