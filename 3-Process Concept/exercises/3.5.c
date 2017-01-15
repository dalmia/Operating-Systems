#include <stdio.h>
#include <unistd.h>

int main()
{
pid_t pid;
    int i;
    int c=1;

    for (i=0; i<2; i++){
        c++;
        pid=fork();
    }
    printf("PID %d\n", pid);
    if(pid != 0){
        wait(NULL);
        printf("%d\n", c);
    }
    return 0;
}
