#ifndef __TREE_H__
#define __TREE_H__

typedef struct TreeNode {
  int data;
  int height;
  struct TreeNode *parent;
  struct TreeNode *left;
  struct TreeNode *right;
} TreeNode;

typedef struct Tree {
  TreeNode *root;
  TreeNode *(*insert)(struct Tree *, int);
  void (*print)(struct Tree *);
} Tree;

extern Tree initTree(int count, ...);

#endif // !__TREE_H__
