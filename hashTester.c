#include <stdlib.h>
#include <stdio.h>
#include "myHash.h"
#include "Hashtable_Stuff/hashtable.h"
#include "Hashtable_Stuff/hashtable_itr.h"

typedef struct Quad {
  int x;
  int y;
  int z;
  int w;
} Quad;

typedef struct hashtable hashtable;

Quad* newQuad(int a, int b, int c, int d) {
  Quad* q = malloc(sizeof(Quad));
  q->x = a;
  q->y = b;
  q->z = c;
  q->w = d;
  return q;
}

unsigned int hashQuad(Quad* q) {
  size_t s = sizeof(Quad);
  return SuperFastHash((char*) q, s);
}

int equalQuads(Quad* a, Quad* b) {
  return(a->x == b->x &&
          a->y == b->y &&
          a->z == b-> z &&
          a->w == b->w);
}

int main() {
  Quad* a = newQuad(1, 2, 3, 4);
  Quad* b = newQuad(5, 5, 5, 5);
  Quad* c = newQuad(5, 15, 20, 25);

  int vala = 1;
  int valb = 2;
  int valc = 74;

  hashtable* quads = create_hashtable(20, hashQuad, equalQuads);

  if(hashtable_insert(quads, a, &vala)) {
    printf("Inserted\n");
  }
  if(hashtable_insert(quads, b, &valb)) {
    printf("Inserted\n");
  }
  if(hashtable_insert(quads, c, &valc)) {
    printf("Inserted\n");
  }

  int* vals[10];

  Quad* d = newQuad(1, 2, 3, 4);
  Quad* e = newQuad(5, 5, 5, 5);
  Quad* f = newQuad(5, 15, 20, 25);
  vals[0] = hashtable_search(quads, d);
  vals[1] = hashtable_search(quads, e);
  vals[2] = hashtable_search(quads, f);


  Quad* g = newQuad(5, 5, 5, 4);
  Quad* h = newQuad(5, 15, 20, 20);

  vals[3] = hashtable_search(quads, g);
  vals[4] = hashtable_search(quads, h);

  for(int i = 0; i < 5; i++) {
    if(vals[i]) {
      printf("The value of the %dth value is %d\n", i, *vals[i]);
    }
    else {
      printf("There is no %dth key\n", i);
    }
  }
  return 0;
}
