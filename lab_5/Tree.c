//
// Tree.c
// 03.12.18
//

#include "Tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

// Binary search in a tree
tree *search(tree *parent, char* value) {
    if (parent == NULL) {
        return NULL;
    }

    if (strcmp(value,parent->value)==0) {
        return parent;
    }

    if (strcmp(value,parent->value)<0) {
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
void insert(tree **root, char* value, tree *parent) {
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

    if (strcmp(value, (*root)->value) < 0) {
        insert(&((*root)->left), value, *root);
    } else {
        insert(&((*root)->right), value, *root);
    }
}

tree * _delete(tree *t, char* x) {
    tree * tmp;
    if (t == NULL) {
        return t;
    }
    if (strcmp(x, t->value)<0) {
        t->left = _delete(t->left, x);
    } else {
        if (strcmp(x, t->value)>0) {
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

// Get key
char* get_key(char *prompt) {

    printf("%s: \n", prompt);

    int capacity_app_elem = 3;
    int size_app_elem = 0;
    char* app_element = ( char* ) malloc( sizeof( char ) * capacity_app_elem );
    char c;
    while ( 1 ) {
        if ( size_app_elem == capacity_app_elem ) {
            capacity_app_elem *= 2;
            app_element = ( char* ) realloc( app_element, sizeof( char ) * capacity_app_elem );
        }
        c = getchar();
        if ( c == '\n' || c == ' ' ) break;
        app_element[size_app_elem] = c;
        size_app_elem++;
    }
    app_element[size_app_elem] = '\0';



    return app_element;
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
    printf("%s\n",root->value);
    printTree(root->left, tabs+1);
}