#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include <stdlib.h>

#define STR_SIZE 500
#define ARR_SIZE 100

typedef struct mapNode {
  unsigned char *key;
  int *val;
  struct mapNode *next;
  struct mapNode *parent;
} mapNode;

typedef struct Hashmap {
  mapNode *arr[ARR_SIZE];
  void (*put)(struct Hashmap *, unsigned char *, int *);
  int *(*get)(struct Hashmap *, unsigned char *);
  void (*remove)(struct Hashmap *, unsigned char *);
} Hashmap;

extern Hashmap init_hashmap();

#endif // !__HASHMAP_H__
