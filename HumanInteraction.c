#include "HumanInteraction.h"
#define MAX_DIGITS 20


int getYesNo(char* prompt) {
  printf("%s ", prompt);
  fflush(stdin);
  char str[MAX_DIGITS];
  fgets(str, MAX_DIGITS, stdin);
  int x;
  sscanf(str, "%d", &x);
  return x;
}

int* getNTuple(int n, char* prompt) {
  int* tuple = malloc(n * sizeof(int));
  for(int i = 0; i < n; i++) {
    printf("%s X_%d: ", prompt, i);
    fflush(stdin);
    char str[MAX_DIGITS];
    fgets(str, MAX_DIGITS, stdin);
    int x;
    sscanf(str, "%d", &x);
    tuple[i] = x;
  }
  return tuple;
}

int* getDims(int* n) {
  printf("Enter number of dimensions: ");
  fflush(stdin);
  char str[MAX_DIGITS];
  fgets(str, MAX_DIGITS, stdin);
  sscanf(str, "%d", n);
  return getNTuple(*n, "Enter length along dimension");
}

// //n is number of dimensions
// Move* getHumanMove(int n) {
//   Move* m = malloc(sizeof(Move));
//   m->n = n;
//   m->coords = getNTuple(n, "Enter coordinate");
//   return m;
// }

// Move* getValidHumanMove(Pos* p) {
//   int madeValid = 0;
//   Move* hm;
//   int numMoves;
//   Move* mList = getMoves(p, &numMoves);
//   while(!madeValid) {
//     hm = getHumanMove(p->n);
//     for(int i = 0; i < numMoves; i++) {
//       if(moveEquals(hm, &mList[i])) {
//         madeValid = 1;
//       }
//     }
//     if(!madeValid) {
//       printf("That's not a valid move, you buffoon!\n");
//       freeMove(hm);
//     }
//   }
//   freeMovesList(mList, numMoves);
//   return hm;
// }
