/* Script to demonstrate deadlock.
   On running this, nothing will be printed as both the threads are waiting
   to get the lock acquired by the other process. Hence, never enter their
   critical sections. Changing both the executing functions to be the same,
   essentially makes it deadlock-free.
*/

#include <pthread.h>
#include <stdio.h>

/* function that the first thread will execute in */
void *do_work_one(void *param);
/* function that the second thread will execute in */
void *do_work_two(void *param);

/* Create and initialize the mutex locks */
pthread_mutex_t first_mutex, second_mutex;

int main(){
    pthread_t tid_1, tid_2; /* the thread identifiers */
    pthread_attr_t attr_1, attr_2; /* thread attributes */

    /* initialize the mutex locks */
    pthread_mutex_init(&first_mutex, NULL);
    pthread_mutex_init(&second_mutex, NULL);

    /* initialize the thread attributes */
    pthread_attr_init(&attr_1);
    pthread_attr_init(&attr_2);

    /* Create thread one */
    pthread_create(&tid_1, &attr_1, do_work_one, NULL);

    /* Create thread two */
    pthread_create(&tid_2, &attr_2, do_work_two, NULL);

    /* Wait for the two threads to complete */
    pthread_join(tid_1, NULL);
    pthread_join(tid_2, NULL);
}

/* function that the first thread will execute in */
void *do_work_one(void *param){
    /* Get the first mutex lock if it's available */
    pthread_mutex_lock(&first_mutex);
    /* Get the second mutex lock if it's available */
    pthread_mutex_lock(&second_mutex);

    /* Critical section start */
    printf("Inside Thread 1\n");
    /* Critical section ends */

    /* Release the locks */
    pthread_mutex_unlock(&first_mutex);
    pthread_mutex_unlock(&second_mutex);

    /* Exit the thread */
    pthread_exit(0);
}

/* function that the second thread will execute in */
void *do_work_two(void *param){
    /* Get the second mutex lock. Note the order of waiting on the locks.
       The second thread gets the second mutex lock and waits for the first
       mutex lock, which is held by the first thread, which is waiting for the
       second mutex lock to be released. This leads to DEADLOCK.
    */
    pthread_mutex_lock(&second_mutex);
    /* Get the first mutex lock */
    pthread_mutex_lock(&first_mutex);

    /* Critical section starts */
    printf("Inside thread 2\n");
    /* Critical section ends */

    /* Release the locks */
    pthread_mutex_unlock(&second_mutex);
    pthread_mutex_unlock(&first_mutex);

    /* Exit the thread */
    pthread_exit(0);
}
