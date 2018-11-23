//main.c

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/mman.h>

#include "vector.h"
#include "vector.cpp"

char read_command() {
    char command;
    do {
        command = getchar();
    } while ( command == '\n' || command == ' ' );
    return command;
}

void read_string(char* area) {
    char c;
    do {
        c = getchar();
    }
    while(c == ' ' || c == '\n');
    int size_app_elem = 1;

    while ( true ) {

        area[size_app_elem] = c;
        size_app_elem++;
        c = getchar();
        if ( c == '\n' || c == ' ' ) break;

    }
    area[size_app_elem] = '\0';
}

void help_function() {
    printf( "command:\n" );
    printf( "<h>\thelp\n" );
    printf( "<q>\texit\n" );
    printf( "<a>\tappend element into array\n" );
    printf( "<p>\tprint array\n" );
    printf( "<d> <index>\tdelete element in array by index\n" );
}

int main( int argc, char* argv[] ) {
    int i, id, size;
    char * element;


    char *area;
    if ((area = mmap (0, sizeof(char) * 100, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0))
        == MAP_FAILED) {
        printf ("mmap error for input");
        return 0;
    }

    //sig set
    int sig;
    pid_t parent_id;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1); //добавляем USR1 в набор
    sigaddset(&set, SIGUSR2); //добавляем USR2 в набор
    sigprocmask(SIG_BLOCK, &set, NULL);
    parent_id = getpid(); //запомним id процесса - предка




    // create a pipe
    int my_pipe[2];
    if ( pipe( my_pipe ) == -1 ) {
        perror( "Error creating pipe\n" );
    }

    // fork
    pid_t child_id;
    child_id = fork();
    if ( child_id == -1 ) {
        perror( "Fork error\n" );
    }

    if ( child_id == 0 ) // child process
    {

        if ( close( my_pipe[0] ) < 0 ) { // child doesn't read
            perror( "Failed to close pipe descriptors" );
        }

        char command;
        printf( "Write a command(<h> for help):\n" );
        while ( true ) {
            command = read_command();

            if ( command == EOF ) {
                command = 'q';
                write( my_pipe[1], &command, 1 );
                kill(parent_id, SIGUSR1);
                break;
            }

            area[0] = command;

            switch ( command ) {
                case 'a':
                    printf( "write element:\n" );
                    read_string(area);
                    break;
                case 'd':

                    read_string(area);
                    break;
                case 'q':
                    if ( close( my_pipe[1] ) < 0 ) {
                        perror( "Failed to close pipe descriptors" );
                    }
                    kill(parent_id, SIGUSR1);
                    return 0;
                case 'p':
                case 'h':
                    break;
                default:
                    printf( "wrong command\n" );
                    break;

            }
            kill(parent_id, SIGUSR1);
            sigwait(&set, &sig);
        }


    } else // parent process
    {


        vector v;
        vector_init( &v );

        if ( close( my_pipe[1] ) < 0 ) { // parent doesn't write
            perror( "Failed to close pipe descriptors" );
        }

        while ( true ) {
            sigwait(&set, &sig);

            if ( area[0] == EOF ) {
                break;
            }

            switch ( area[0] ) {
                case 'a':
                    size = strlen(area);
                    element = ( char* ) malloc( sizeof( char ) * size );
                    for ( i = 1; i < size; ++i ) {
                        element[i-1] = area[i];
                    }
                    element[i] = '\0';
                    vector_add( &v, element );
                    break;
                case 'd':
                    size = strlen(area);
                    id = area[1] - '0';
                    for ( i = 2; i < size; ++i ) {
                        id = (id * 10) + (area[i] - '0');
                    }
                    //printf("%d\n", id);
                    vector_delete( &v, id );
                    break;
                case 'p':
                    printf( "vector:\n" );
                    for ( i = 0; i < vector_count( &v ); i++ ) {
                        element = vector_get( &v, i );
                        printf( "%s\n", element );
                    }
                    break;
                case 'q':
                    vector_free( &v );
                    if ( close( my_pipe[0] ) < 0 ) {
                        perror( "Failed to close pipe descriptors" );
                    }
                    return 0;
                case 'h':
                    help_function();
                    break;
                default:
                    printf( "wrong command\n" );
                    break;

            }
            printf( "Write a command(<h> for help):\n" );
            kill(child_id, SIGUSR2);
        }

        if ( close( my_pipe[0] ) < 0 ) {
            perror( "Failed to close pipe descriptors" );
        }


    }

    return 0;
}