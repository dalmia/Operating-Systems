#include <omp.h>
#include <stdio.h>

int main()
{
    /* sequential code */

    #pragma omp parallel
    {
        printf("Parallel block\n");
    }

    /* sequential block */

    return 0;
}
