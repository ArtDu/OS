#include <stdlib.h>
#include <stdio.h>
#include "Tree.h"
#include <dlfcn.h>


int main(int argc, char *argv[]) {

    tree* (*search)(tree *parent, char* value);
    tree* (*min)(tree *parent);
    tree* (*max)(tree *parent);
    void (*printTree)(tree *root, int tabs);
    void (*traverse)(tree *parent);
    void (*insert)(tree **root, char* value, tree *parent);
    tree* (*_delete)(tree *t, char* x);
    int (*get_int)(char *prompt);
    char* (*get_key)(char *prompt);
    void (*flush_stdin)(void);

    char *err;
    void *libHandle;
    libHandle = dlopen("libtree.so", RTLD_LAZY);
    if (!libHandle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

    search = dlsym(libHandle, "search");
    min = dlsym(libHandle, "min");
    max = dlsym(libHandle, "max");
    printTree = dlsym(libHandle, "printTree");
    traverse = dlsym(libHandle, "traverse");
    insert = dlsym(libHandle, "insert");
    _delete = dlsym(libHandle, "_delete");
    get_int = dlsym(libHandle, "get_int");
    flush_stdin = dlsym(libHandle, "flush_stdin");
    get_key = dlsym(libHandle, "get_key");


    int cmd = 0;
    char* value;
    tree *root = NULL;

    do {
        printf("Commands\n\n"
               "1 - Insert\n"
               "2 - Search\n"
               "3 - Delete\n"
               "4 - Print tree\n"
               "0 - Quit\n\n");

        cmd = get_int("Command");

        switch (cmd) {
            case 1:
                value = get_key("value to insert");
                insert(&root, value, NULL);
                break;
            case 2:
                value = get_key("value to find");
                if (search(root, value) == NULL) {
                    printf("%s is not found in tree!\n", value);
                } else {
                    printf("%s is found in tree!\n", value);
                }
                break;
            case 3:
                value = get_key("value to delete");
                root = _delete(root, value);
                break;
            case 4:
                printf("tree:\n");
                printTree(root, 0);
                break;
        }

    } while (cmd != 0);

    return 0;
}
