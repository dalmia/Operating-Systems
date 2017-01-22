#include <stdio.h>

int main(void)
{
    int p1[10];
    int *p2[10];
    int (*p3)[10];

    printf("sizeof(int)   = %d\n", (int)sizeof(int));
    printf("sizeof(int *) = %d\n", (int)sizeof(int *));
    printf("sizeof(p1)    = %d\n", (int)sizeof(p1));
    printf("sizeof(p2)    = %d\n", (int)sizeof(p2));
    printf("sizeof(p3)    = %d\n", (int)sizeof(p3));

    return 0;
}
