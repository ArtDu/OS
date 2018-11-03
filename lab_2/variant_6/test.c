#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


int main(int argc, char* argv[])
{

    char* arguments[] = {"ls", NULL};
    int wstatus;

    pid_t child_id;
    child_id = fork();
    if(child_id == -1)
    {
        fprintf(stderr, "Fork error\n");
    }

    if(child_id == 0) // child process
    {

        execvp("ls", arguments);

        fprintf(stderr, "Exec failed\n");
    }
    else
    {

        wait( &wstatus );
    }
}