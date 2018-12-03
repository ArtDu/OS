#include <stdlib.h>
#include <stdio.h>
#include "Tree.h"


int main(int argc, char *argv[]) {
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
