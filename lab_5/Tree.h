//
// Tree.h
// 03.12.18
//

#ifndef LAB_5_TREE_H
#define LAB_5_TREE_H

typedef struct tree {
    char* value;
    struct tree *parent;
    struct tree *left;
    struct tree *right;
} tree;

tree *search(tree *parent, char* value);
tree *min(tree *parent);
tree *max(tree *parent);

void printTree(tree *root, int tabs);
void traverse(tree *parent);
void insert(tree **root, char* value, tree *parent);
tree * _delete(tree *t, char* x);

int get_int(char *prompt);
char* get_key(char *prompt);
void flush_stdin(void);


#endif //LAB_5_TREE_H
