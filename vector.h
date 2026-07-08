#ifndef __VECTOR_H__
#define __VECTOR_H__

typedef struct Vector {
  int size;
  int last;
  void **storage;
  void (*push_back)(struct Vector *, void *);
  void (*pop)(struct Vector *);
  void *(*at)(struct Vector *, int);
} Vector;

extern Vector *init_vector();

#endif // !__VECTOR_H__
