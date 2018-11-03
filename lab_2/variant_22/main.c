#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>

#include "vector.h"
#include "vector.cpp"

char read_command() {
    printf("Write a command(<h> for help):\n");
    char command;
    scanf("%c", &command);
    return command;
}

void help_function() {
    printf("command:\n");
    printf("<h>\thelp\n");
    printf("<q>\texit\n");
    printf("<a>\tappend element into array\n");
    printf("<p>\tprint array\n");
    printf("<d> <index>\tdelete element in array by index\n");
}

int main( int argc, char* argv[] ) {
    int i, id, size;
    char app_element[101], *element;
    vector v;
    vector_init(&v);

    while ( true ) {

        int wstatus;

        // create a pipe
        int my_pipe[2];
        if ( pipe( my_pipe ) == -1 ) {
            perror("Error creating pipe\n");
        }

        // fork
        pid_t child_id;
        child_id = fork();
        if ( child_id == -1 ) {
            perror("Fork error\n");
        }


        if ( child_id == 0 ) // child process
        {
            if ( close( my_pipe[0]  ) < 0 ) { // child doesn't read
                perror("Failed to close pipe descriptors");
            }
             
            char command;
            command = read_command();

            switch (command) {
                case 'a':
                    printf("write element:\n");
                    scanf("%s", app_element);
                    size = strlen(app_element);
                    write( my_pipe[1], &command, 1 );
                    write( my_pipe[1], &size, sizeof(int) );
                    write( my_pipe[1], app_element, strlen(app_element) );
                    break;
                case 'd':
                    scanf("%d", &id);
                    write( my_pipe[1], &command, 1 );
                    write( my_pipe[1], &id, sizeof(int) );
                    break;
                case 'p':
                case 'q':
                case 'h':
                    write( my_pipe[1], &command, 1 );
                    break;
                default:
                    write( my_pipe[1], "c", 1 );
                    break;

            }



            
            if ( close( my_pipe[1]  ) < 0 ) {
                perror("Failed to close pipe descriptors");
            }
            break;

        } else // parent process
        {
            if ( wait ( &wstatus ) < 0 ) {
                perror("Failed in wait function");
            }

            if ( close( my_pipe[1] ) < 0 ) { // parent doesn't write
                perror("Failed to close pipe descriptors");
            }

            char reading_buf;
            do {
                read( my_pipe[0], &reading_buf, 1 );
            }
            while(reading_buf == '\n' || reading_buf == ' ');

            switch (reading_buf) {
                case 'a':
                    read( my_pipe[0], &size, sizeof(int) );
                    element = (char*)malloc(sizeof(char) * size);
                    read( my_pipe[0], element, size );
                    vector_add(&v, element);
                    break;
                case 'd':
                    read( my_pipe[0], &id, sizeof(int) );
                    vector_delete(&v, id);
                    break;
                case 'p':
                    printf("vector:\n");
                    for (i = 0; i < vector_count(&v); i++) {
                        element = vector_get(&v, i);
                        printf("%s\n", element);
                    }
                    break;
                case 'q':
                    vector_free(&v);
                    if ( close( my_pipe[0] ) < 0 ) {
                        perror("Failed to close pipe descriptors");
                    }
                    return 0;
                case 'h':
                    help_function();
                    break;
                default:
                    printf("wrong command\n");
                    break;

            }

            if ( close( my_pipe[0] ) < 0 ) {
                perror("Failed to close pipe descriptors");
            }

        }
    }
    return 0;
}