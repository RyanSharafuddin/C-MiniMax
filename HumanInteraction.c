#include "HumanInteraction.h"
#define MAX_DIGITS 20

int getInt() {
  char str[MAX_DIGITS];
  fgets(str, MAX_DIGITS, stdin);
  int x;
  sscanf(str, "%d", &x);
  return x;
}

int getYesNo(char* prompt) {
  printf("%s ", prompt);
  fflush(stdin);
  return getInt();
}

int* getNTuple(int n, char* prompt) {
  int* tuple = malloc(n * sizeof(int));
  for(int i = 0; i < n; i++) {
    printf("%s X_%d: ", prompt, i);
    fflush(stdin);
    tuple[i] = getInt();
  }
  return tuple;
}

int* getDims(int* n) {
  printf("Enter number of dimensions: ");
  fflush(stdin);
  *n = getInt();
  return getNTuple(*n, "Enter length along dimension");
}
