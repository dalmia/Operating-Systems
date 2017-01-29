#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    char *v = "Aman";
    char *v2 = "Aan";
    char **c = malloc(2*sizeof(char*));
    char ***a = malloc(sizeof(char**));
    *c = v;
    c++;
    *c = v2;
    c--;
    printf("%s\n", *c);
    c++;
    printf("%s\n", *c);

    return 0;
}
