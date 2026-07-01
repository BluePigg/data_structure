#include <stdint.h>
#include <stdlib.h>

#define STR_SIZE 500
#define ARR_SIZE 100

typedef struct mapNode {
  void *key;
  void *val;
  struct mapNode *next;
  struct mapNode *parent;
} mapNode;

typedef struct Hashmap {
  mapNode *arr[ARR_SIZE];
  void (*put)(struct Hashmap *, void *, void *);
  void *(*get)(struct Hashmap *, void *);
  void (*remove)(struct Hashmap *, void *);
  unsigned long (*get_hash)(void *);
  int (*compare_keys)(void *, void *);
} Hashmap;

unsigned long hashstr(void *key) {
  unsigned char *str = (unsigned char *)key;
  unsigned long hash = 5381;
  int c = 1;
  while (c) {
    c = *str++;

    hash = (hash << 5) + hash + c;
  }
  return hash;
}

unsigned long hashint(void *key) {
  unsigned int x = *(unsigned int *)key;
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = (x >> 16) ^ x;
  return x;
}

int cmpstr(void *fst_p, void *snd_p) {
  unsigned char *fst = (unsigned char *)fst_p;
  unsigned char *snd = (unsigned char *)snd_p;
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

int cmpint(void *fst_p, void *snd_p) { return *(int *)fst_p == *(int *)snd_p; }

static mapNode *createNode(void *key, void *val, mapNode *prev) {
  mapNode *node = (mapNode *)malloc(sizeof(mapNode));
  node->key = key;
  node->val = val;
  if (prev != NULL) {
    prev->next = node;
    node->parent = prev;
  }
  return node;
}

static void _put(Hashmap *map, void *key, void *val) {
  int idx = map->get_hash(key) % ARR_SIZE;
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

static mapNode *get_node_w_key(Hashmap *map, void *key) {
  int idx = map->get_hash(key) % ARR_SIZE;
  mapNode *node = map->arr[idx];
  while (node != NULL) {
    if (map->compare_keys(node->key, key)) {
      return node;
    }
    node = node->next;
  }
  return NULL;
}

static void *_get(Hashmap *map, void *key) {
  mapNode *node = get_node_w_key(map, key);
  return node == NULL ? NULL : node->val;
}

static void _remove(Hashmap *map, void *key) {
  int idx = map->get_hash(key) % ARR_SIZE;
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

Hashmap init_hashmap(unsigned long (*gethash)(void *),
                     int (*comparekeys)(void *, void *)) {
  Hashmap map;
  for (int i = 0; i < ARR_SIZE; i++) {
    map.arr[i] = NULL;
  }
  map.put = _put;
  map.get = _get;
  map.remove = _remove;
  map.get_hash = gethash;
  map.compare_keys = comparekeys;

  return map;
}
