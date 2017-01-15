#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main()
{
pid_t pid,pid1;

    pid = fork();
    if(pid == 0){
        pid1 = getpid();
        printf("Child: pid = %d\n", pid1);

        /* pass exit status */
        exit(2);
    }
    else if(pid > 0){
        // int status;
        // pid = wait(&status);

        /* get parent pid */
        pid1 = getpid();
        printf("Parent: pid = %d\n", pid1);

        /* execute ps -l from the parent */
        execlp("/bin/ps", "ps", "-l", NULL);

        //execlp("/bin/kill", "kill", "-9", getppid(), NULL);
        //printf("Exit status: status = %d\n", status);
    }

    return 0;
}
