#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <fcntl.h>

#define SIZE sizeof(char) * 100

int main (int argc, char *argv[])
{
    int i, id, size;
    char *area;

    //sig set
    int sig, child, parent_id;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1); //добавляем USR1 в набор
    sigaddset(&set, SIGUSR2); //добавляем USR2 в набор
    sigprocmask(SIG_BLOCK, &set, NULL);
    parent_id = getpid(); //запомним id процесса - предка


    if ((area = mmap (0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0))
    == MAP_FAILED) {
        printf ("mmap error for input");
        return 0;
    }

    

    // fork
    pid_t child_id;
    child_id = fork();
    if ( child_id == -1 ) {
        perror( "Fork error\n" );
    }

    while (true) {

        if ( child_id == 0 ) // child process
        {
            do {
                area[0] = getchar();
            }
            while(area[0] == '\n');
            kill(parent_id, SIGUSR1);
            if (area[0] == 'q') {
                break;
            }
            sigwait(&set, &sig);

        } else // parent process
        {
            sigwait(&set, &sig);
            printf("%c\n", area[0]);
            if (area[0] == 'q') {
                break;
            }
            kill(child_id, SIGUSR2);
        }
    }


    return 0;

} /* main */  