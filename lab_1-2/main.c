#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


int main(int argc, char* argv[])
{
    char* arguments[] = {argv[1], NULL};
    int wstatus;

    int my_pipe[2];
    if(pipe(my_pipe) == -1)
    {
        fprintf(stderr, "Error creating pipe\n");
    }

    pid_t child_id;
    child_id = fork();
    if(child_id == -1)
    {
        fprintf(stderr, "Fork error\n");
    }

    if(child_id == 0) // child process
    {
        close(my_pipe[0]); // child doesn't read
        dup2(my_pipe[1], 1); // redirect stdout

        execvp(argv[1], arguments);

        fprintf(stderr, "Exec failed\n");
    }
    else
    {
        close(my_pipe[1]); // parent doesn't write

        char reading_buf[1];
        while(read(my_pipe[0], reading_buf, 1) > 0)
        {
            if ( reading_buf[0] == '\t' ) {
                *reading_buf = ' ';
                write(1, reading_buf, 1);
                write(1, reading_buf, 1);
            }
            else if ( reading_buf[0] == ' ' ) {
                write(1, reading_buf, 1);
            }
            else {
                write(1, reading_buf, 1);
            }
        }
        close(my_pipe[0]);
        wait( &wstatus );
    }
}