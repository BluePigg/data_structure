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

static unsigned long get_hash(unsigned char *str) {
  unsigned long hash = 5381;
  int c = 1;
  while (c) {
    c = *str++;

    hash = (hash << 5) + hash + c;
  }
  return hash;
}

static int cmpstr(unsigned char *fst, unsigned char *snd) {
  for (int i = 0; i < STR_SIZE; i++) {
    if (fst[i] == snd[i]) {
      if (fst[i] == 0) {
        return 1;
      }
    } else {
      return 0;
    }
  }
  return 0;
}

static mapNode *createNode(unsigned char *key, int *val, mapNode *prev) {
  mapNode *node = (mapNode *)malloc(sizeof(mapNode));
  node->key = key;
  node->val = val;
  if (prev != NULL) {
    prev->next = node;
    node->parent = prev;
  }
  return node;
}

static void _put(Hashmap *map, unsigned char *key, int *val) {
  int idx = get_hash(key) % ARR_SIZE;
  mapNode *existingNode = map->arr[idx];
  while (existingNode != NULL && existingNode->next != NULL) {
    existingNode = existingNode->next;
  }
  mapNode *node =
      createNode(key, val, existingNode == NULL ? NULL : existingNode);
  if (existingNode == NULL) {
    map->arr[idx] = node;
  }
}

static mapNode *get_node_w_key(Hashmap *map, unsigned char *key) {
  int idx = get_hash(key) % ARR_SIZE;
  mapNode *node = map->arr[idx];
  while (node != NULL) {
    if (cmpstr(node->key, key)) {
      return node;
    }
    node = node->next;
  }
  return NULL;
}

static int *_get(Hashmap *map, unsigned char *key) {
  mapNode *node = get_node_w_key(map, key);
  return node == NULL ? NULL : node->val;
}

static void _remove(Hashmap *map, unsigned char *key) {
  int idx = get_hash(key) % ARR_SIZE;
  mapNode *node = get_node_w_key(map, key);
  if (node != NULL) {
    mapNode *prev = node->parent;
    mapNode *next = node->next;
    if (prev) {
      prev->next = next == NULL ? NULL : next;
    } else {
      map->arr[idx] = next == NULL ? NULL : next;
    }
    if (next) {
      next->parent = prev == NULL ? NULL : prev;
    }
    free(node);
  }
}

Hashmap init_hashmap() {
  Hashmap map;
  for (int i = 0; i < ARR_SIZE; i++) {
    map.arr[i] = NULL;
  }
  map.put = _put;
  map.get = _get;
  map.remove = _remove;

  return map;
}
