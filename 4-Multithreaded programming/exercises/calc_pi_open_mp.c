#include <sys/types.h>
#include <stdio.h>
#include <omp.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NUM_COUNT 10000000

int c=0;
int tot=0;
int i;

int main(){

    srand(time(NULL));

    #pragma omp parallel for
    for(i=0; i<NUM_COUNT; i++){
        tot++;
        double x_ran = ((double)(2)/RAND_MAX) * rand() - 1;
        double y_ran = ((double)(2)/RAND_MAX) * rand() - 1;
        double dist = sqrt(pow(x_ran,2)+pow(y_ran,2));
        if(dist<1) c++;
    }

    double pi = 4 * ((double) c)/tot;
    printf("pi = %f\n", pi);
}
