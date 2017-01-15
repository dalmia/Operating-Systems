#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
int n;
pid_t pid;

    printf("Enter N:");
    scanf("%d", &n);

    /* create child process */
    pid = fork();

    if(pid == 0){ /* child process */
        while(n > 1){
            printf("%d, ", n);
            if(n%2 == 0) n = n/2;
            else n = 3*n + 1;
        }
        printf("%d\n", n);
    }
    else if(pid > 0){ /* parent process */
        wait(NULL);
    }

    return 0;
}
