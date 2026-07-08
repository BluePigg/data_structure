#include <stdlib.h>

typedef struct Vector {
  int size;
  int last;
  void **storage;
  void (*push_back)(struct Vector *, void *);
  void (*pop)(struct Vector *);
  void *(*at)(struct Vector *, int);
} Vector;

static void _push_back(Vector *vector, void *data) {
  vector->last++;
  if (vector->size <= vector->last) {
    vector->size *= 2;
    if (vector->storage == NULL) {
      vector->storage =
          (void **)realloc(vector->storage, sizeof(data) * vector->size);
    }
  }
  vector->storage[vector->last] = data;
}

static void *_at(Vector *vector, int idx) { return vector->storage[idx]; }

static void _pop(Vector *vector) {
  vector->storage[vector->last] = NULL;
  vector->last--;
}

Vector *init_vector() {
  Vector *vector = (Vector *)malloc(sizeof(Vector));
  vector->storage = (void **)malloc(sizeof(void *));
  vector->size = 1;
  vector->last = -1;
  vector->push_back = _push_back;
  vector->at = _at;
  vector->pop = _pop;

  return vector;
}
