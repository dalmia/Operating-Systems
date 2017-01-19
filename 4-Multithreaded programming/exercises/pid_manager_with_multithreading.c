#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define llim 300
#define ulim 5000
#define NTHREADS 50

int map[ulim+1];
int i;

void allocate_map();
int allocate_pid();
void release_pid(int);
void *runner();
void *runner2();

int main()
{
    pthread_t tid[NTHREADS];
    pthread_attr_t attr;

    allocate_map();
    srand(time(NULL));
    pthread_attr_init(&attr);

    int j=0;
    for(j=0; j<NTHREADS; j++){
        printf("%d\n", j);
        if(j%2 == 0)
            pthread_create(&tid[j], &attr, runner, NULL);
        else
            pthread_create(&tid[j], &attr, runner2, NULL);
    }

    for(j=0; j<NTHREADS; j++){
        pthread_join(tid[j], NULL);
    }
}

void allocate_map(){
    for(i=0; i<=ulim; i++) map[i]=0;
}

int allocate_pid(){
    for(i=llim; i<=ulim; i++){
        if(map[i]==0){
            map[i] = 1;
            return i;
        }
    }
    return -1;
}

void release_pid(int pid){
    map[pid] = 0;
}

void *runner(){
    int pid = allocate_pid();
    printf("Acquired pid = %d\n", pid);

    int sec = rand() % 10;
    printf("Sleeping for %d secs\n", sec);
    sleep(sec);

    printf("Released pid = %d\n", pid);
    pthread_exit(0);
}

void *runner2(){
    int pid = allocate_pid();
    printf("Acquired pid1 = %d\n", pid);

    int sec = rand() % 10;
    printf("Sleeping for %d secs\n", sec);
    sleep(sec);

    printf("Released pid1 = %d\n", pid);
    pthread_exit(0);
}
