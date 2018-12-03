//
// Tree.h
// 03.12.18
//

#ifndef LAB_5_TREE_H
#define LAB_5_TREE_H

typedef struct tree {
    int value;
    struct tree *parent;
    struct tree *left;
    struct tree *right;
} tree;

tree *search(tree *parent, int value);
tree *min(tree *parent);
tree *max(tree *parent);

void printTree(tree *root, int tabs);
void traverse(tree *parent);
void insert(tree **root, int value, tree *parent);
tree * _delete(tree *t, int x);

int get_int(char *prompt);
void flush_stdin(void);


#endif //LAB_5_TREE_H
