#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct NODE {
  int data;
  struct NODE *next;
} Node;

typedef struct LINKED_LIST {
  Node *begin;
  Node *(*insert)(Node *, int);
  Node *(*search_nd)(struct LINKED_LIST *, int);
  int (*search_va)(struct LINKED_LIST *, int);
} Linked_list;

extern Linked_list init_linked_list(int count, ...);

#endif // linked_list
