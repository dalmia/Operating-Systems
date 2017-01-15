#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main()
{
/* size (in bytes) of shared memory object */
const int SIZE = 8192;
/* name of the shared memory object */
const char *name = "OS";

/* shared memory file descriptor */
int shm_fd;
/* pointer to shared memory object */
int *ptr;

int n;
pid_t pid;

    printf("Enter N:");
    scanf("%d", &n);

    /* create the shared memory object */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    /* create child process */
    pid = fork();

    if(pid == 0){ /* child process */
        /* memory map the shared memory object */
        ptr = (int *)mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
        int i=0;
        while(n > 1){
            ptr[i++] = n;
            if(n%2 == 0) n = n/2;
            else n = 3*n + 1;
        }
        ptr[i] = n;
    }
    else if(pid > 0){ /* parent process */
        wait(NULL);
        /* memory map the shared memory object */
        ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
        int i=0;
        while(ptr[i]!=1){
            printf("%d,", ptr[i]);
            i++;
        }
        printf("%d\n", ptr[i]);
    }

    return 0;
}
