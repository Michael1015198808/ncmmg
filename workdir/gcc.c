#include <stdio.h>
int read() {
    int tmp;
    scanf("%d", &tmp);
    return tmp;
}
int write(int tmp) {
    printf("%d\n", tmp);
    return 0;
}
#include "tmp.c"
