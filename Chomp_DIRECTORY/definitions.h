#ifndef DEFINITIONS_H
#define DEFINITIONS_H

struct Pos {
  int n;      //the n in n-dimensional pos. The length of dims
  int* dims;  //array of dimension measurements
  int* rows;  //1D array of rows
  int numRows;  //all space taken up by all dimensions
};

struct Move {
  int* coords;
  int n; //number of dimensions
} Move;

#endif
