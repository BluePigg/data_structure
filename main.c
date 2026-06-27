#include "tree.h"

int main() {
  Tree tree = initTree(0);
  for (int i = 0; i <= 500; i++) {
    tree.insert(&tree, i);
  }
  tree.print(&tree);

  return 0;
}
