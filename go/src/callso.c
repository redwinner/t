// gcc callso.c -L. -lhello -o callso
// LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. ./callso
#include <stdio.h>
#include "libhello.h"

int main() {
    Hello("World");

    int sum = Add(3, 4);
    printf("Add result: %d\n", sum);

    return 0;
}
