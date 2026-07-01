#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
  int idx;
  int data;
  int height;
  struct TreeNode *parent;
  struct TreeNode *left;
  struct TreeNode *right;
} TreeNode;

typedef struct Tree {
  TreeNode *root;
  TreeNode *(*insert)(struct Tree *, int, int);
  void (*print)(struct Tree *);
} Tree;

static TreeNode *createNode(int idx, int data) {
  TreeNode *new = (TreeNode *)malloc(sizeof(TreeNode));
  new->idx = idx;
  new->height = 0;
  new->data = data;
  new->parent = NULL;
  new->left = NULL;
  new->right = NULL;

  return new;
}

static int max(int a, int b) { return a > b ? a : b; }

// use when ll
static TreeNode *_rotate_ll(Tree *tree, TreeNode *node) {
  TreeNode *parent = node->parent;
  if (node->right != NULL) {
    node->right->parent = parent;
  }
  parent->left = node->right;
  node->right = parent;
  node->parent = parent->parent;
  if (parent->parent != NULL) {
    TreeNode *super = parent->parent;
    if (super->left == parent) {
      parent->parent->left = node;
    } else {
      parent->parent->right = node;
    }
  }
  parent->parent = node;
  if (tree->root == parent) {
    tree->root = node;
  }
  int pl = parent->left != NULL ? parent->left->height : -1;
  int pr = parent->right != NULL ? parent->right->height : -1;
  parent->height = 1 + max(pl, pr);

  int nl = node->left != NULL ? node->left->height : -1;
  int nr = node->right != NULL ? node->right->height : -1;
  node->height = 1 + max(nl, nr);

  return node;
}

// use when rr
static TreeNode *_rotate_rr(Tree *tree, TreeNode *node) {
  TreeNode *parent = node->parent;
  if (node->left != NULL) {
    node->left->parent = parent;
  }
  parent->right = node->left;
  node->left = parent;
  node->parent = parent->parent;
  if (parent->parent != NULL) {
    TreeNode *super = parent->parent;
    if (super->right == parent) {
      parent->parent->right = node;
    } else {
      parent->parent->left = node;
    }
  }
  parent->parent = node;
  if (tree->root == parent) {
    tree->root = node;
  }
  int pl = parent->left != NULL ? parent->left->height : -1;
  int pr = parent->right != NULL ? parent->right->height : -1;
  parent->height = 1 + max(pl, pr);

  int nl = node->left != NULL ? node->left->height : -1;
  int nr = node->right != NULL ? node->right->height : -1;
  node->height = 1 + max(nl, nr);
  return node;
}

static TreeNode *_rotate_lr(Tree *tree, TreeNode *node) {
  _rotate_rr(tree, node->right);
  return _rotate_ll(tree, node->parent);
}

static TreeNode *_rotate_rl(Tree *tree, TreeNode *node) {
  _rotate_ll(tree, node->left);
  return _rotate_rr(tree, node->parent);
}

static int _replace_nodes(Tree *tree, TreeNode *node, int inserted_idx) {
  TreeNode *parent = node->parent;

  if (parent == NULL) {
    return 1;
  }
  int lheight = parent->left != NULL ? parent->left->height : -1;
  int rheight = parent->right != NULL ? parent->right->height : -1;
  parent->height = 1 + max(lheight, rheight);

  int bf = lheight - rheight;

  if (bf > 1 && inserted_idx < parent->left->idx) {
    node = _rotate_ll(tree, node);
  } else if (bf < -1 && inserted_idx > parent->right->idx) {
    node = _rotate_rr(tree, node);
  } else if (bf > 1 && inserted_idx > parent->left->idx) {
    node = _rotate_lr(tree, node);
  } else if (bf < -1 && inserted_idx < parent->right->idx) {
    node = _rotate_rl(tree, node);
  }

  return node->parent != parent;
}

static void print_tree_recu(Tree *tree, TreeNode *node, int isleft,
                            char *prefix) {
  if (node == NULL) {
    return;
  }
  char pf[512];
  snprintf(pf, 512, "%s%s", prefix, isleft ? "|   " : "    ");
  print_tree_recu(tree, node->right, 0, pf);
  if (node->parent != NULL) {
    printf("%s%s", prefix, isleft ? "\\-- " : "/-- ");
  }
  printf("%d", node->idx);
  printf("%s", node->height == tree->root->height ? "--|\n" : "\n");
  snprintf(pf, 512, "%s%s", prefix, isleft ? "    " : "|   ");
  pf[0] = ' ';
  print_tree_recu(tree, node->left, 1, pf);
}

static void _print_tree(Tree *tree) {
  print_tree_recu(tree, tree->root, 0, "");
}

static TreeNode *_insert(Tree *tree, int idx, int data) {
  TreeNode *new = createNode(idx, data);
  TreeNode *cur = tree->root;
  if (cur == NULL) {
    tree->root = new;
    return new;
  }
  if (cur->idx == new->idx) {
    return cur;
  }
  while (1) {
    int smaller = idx < cur->idx;
    TreeNode *ncur = smaller ? cur->left : cur->right;
    if (ncur == NULL) {
      if (smaller) {
        cur->left = new;
      } else {
        cur->right = new;
      }
      break;
    } else {
      cur = ncur;
      new->height++;
    }
  }
  new->parent = cur;

  TreeNode *for_rotation = new;
  while (for_rotation != NULL) {
    int rotated = _replace_nodes(tree, for_rotation, new->idx);
    for_rotation = for_rotation->parent;
    if (rotated == 1) {
      break;
    }
  }
  return new;
}

Tree initTree(int count, ...) {
  Tree tree;
  tree.root = NULL;
  tree.insert = _insert;
  tree.print = _print_tree;

  va_list ap;
  va_start(ap, count);

  for (int i = 0; i < count; i++) {
    tree.insert(&tree, va_arg(ap, int), 0);
  }
  va_end(ap);

  return tree;
}
