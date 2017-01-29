#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_THREADS 5

void *runner(void *param);

int main(){
    int i, policy;
    pthread_t tid[NUM_THREADS];
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    if(pthread_attr_getschedpolicy(&attr, &policy) != 0){
        fprintf(stderr, "Unable to get policy\n");
    }else{
        if(policy == SCHED_OTHER) printf("SCHED_OTHER\n");
        else if(policy == SCHED_RR) printf("SCHED_RR\n");
        else if(policy == SCHED_FIFO) printf("SCHED_FIFO\n");
    }

    if(pthread_attr_setschedpolicy(&attr, SCHED_FIFO) != 0){
        fprintf(stderr, "Error setting policy\n");
    }

    for(i = 0; i < NUM_THREADS; i++){
        pthread_create(&tid[i], &attr, runner, NULL);
    }

    for(i = 0; i < NUM_THREADS; i++){
        pthread_join(tid[i], NULL);
    }
}

void *runner(void *param){
    printf("Inside runner\n");
    sleep(3);
    printf("Exiting runner\n");

    pthread_exit(0);
}
