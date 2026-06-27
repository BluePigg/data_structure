#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct LLNODE {
  int data;
  struct LLNODE *next;
} LLNode;

typedef struct LINKED_LIST {
  LLNode *begin;
  LLNode *(*insert)(LLNode *, int);
  LLNode *(*search_nd)(struct LINKED_LIST *, int);
  int (*search_va)(struct LINKED_LIST *, int);
} Linked_list;

static LLNode *_insert(LLNode *node, int data) {
  LLNode *new = (LLNode *)malloc(sizeof(LLNode));
  new->data = data;
  new->next = NULL;
  if (node->next == NULL) {
    node->next = new;
  } else {
    LLNode *next = node->next;
    node->next = new;
    new->next = next;
  }
  return new;
}

static LLNode *_search_nd(Linked_list *list, int index) {
  LLNode *nd = list->begin;
  for (int i = 0; i < index; i++) {
    if (nd->next != NULL) {
      nd = nd->next;
    } else {
      break;
    }
  }
  return nd;
}

static int _search_va(Linked_list *list, int index) {
  LLNode *nd = list->search_nd(list, index);
  return nd->data;
}

Linked_list init_linked_list(int count, ...) {
  Linked_list list;
  list.begin = NULL;
  list.insert = _insert;
  list.search_nd = _search_nd;
  list.search_va = _search_va;

  va_list ap;
  va_start(ap, count);

  LLNode *current;
  for (int i = 0; i < count; i++) {
    if (list.begin == NULL) {
      LLNode *first = (LLNode *)malloc(sizeof(LLNode));
      first->data = va_arg(ap, int);
      first->next = NULL;
      list.begin = first;
      current = first;
    } else {
      current = list.insert(current, va_arg(ap, int));
    }
  }

  va_end(ap);

  return list;
}
