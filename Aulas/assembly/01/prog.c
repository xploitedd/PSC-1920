#include<stdio.h>

int func(int a, int b);

int main(int argc, char *argv[]) {
    int res = func(3, 4);
    printf("res: %d\n", res);
    return 0;
}