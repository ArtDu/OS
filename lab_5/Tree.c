//
// Tree.c
// 03.12.18
//

#include "Tree.h"
#include <stdlib.h>
#include <stdio.h>

// Binary search in a tree
tree *search(tree *parent, int value) {
    if (parent == NULL) {
        return NULL;
    }

    if (value == parent->value) {
        return parent;
    }

    if (value < parent->value) {
        return search(parent->left, value);
    } else {
        return search(parent->right, value);
    }
}

// Find minimum item in the tree
tree *min(tree *parent) {
    if (parent == NULL) {
        return NULL;
    }

    tree *min = parent;
    while (min->left != NULL) {
        min = min->left;
    }

    return min;
}

// Find maximum item in the tree
tree *max(tree *parent) {
    if (parent == NULL) {
        return NULL;
    }

    tree *max = parent;
    while (max->right != NULL) {
        max = max->right;
    }

    return max;
}

// Walk through the tree using In-Order
void traverse(tree *parent) {
    if (parent != NULL) {
        traverse(parent->left);
        printf("(%d)->", parent->value);
        traverse(parent->right);
    }
}

// Insert an element in the tree
void insert(tree **root, int value, tree *parent) {
    tree *p = NULL;

    // Insert to an empty tree
    if (*root == NULL) {
        p = malloc(sizeof(tree));

        p->value = value;
        p->left = NULL;
        p->right = NULL;
        p->parent = parent;

        *root = p;

        return;
    }

    if (value < (*root)->value) {
        insert(&((*root)->left), value, *root);
    } else {
        insert(&((*root)->right), value, *root);
    }
}

tree * _delete(tree *t, int x) {
    tree * tmp;
    if (t == NULL) {
        return t;
    }
    if (x < t->value) {
        t->left = _delete(t->left, x);
    } else {
        if (x > t->value) {
            t->right = _delete(t->right, x);
        } else {
            if (t->left && t->right) {
                tmp = min(t->right);
                t->value = tmp->value;
                t->right = _delete(t->right, t->value);
            } else {
                tmp = t;
                if (t->left == NULL) {
                    t = t->right;
                } else {
                    if (t->right == NULL) {
                        t = t->left;
                    }
                    free(tmp);
                }
            }
        }
    }
    return t;
}

// Get int
int get_int(char *prompt) {
    int value = 0;

    printf("%s: \n", prompt);
    scanf("%d", &value);

    flush_stdin();

    return value;
}

// Flush input buffer
void flush_stdin(void) {
    while (getc(stdin) != '\n');
}

void printTree(tree *root, int tabs) {
    if(root == NULL) return;

    printTree(root->right, tabs+1);
    for ( int i = 0; i < tabs; ++i ) {
        printf("\t");
    }
    printf("%d\n",root->value);
    printTree(root->left, tabs+1);
}