#include<stdio.h>

// a1 .. a6: rsi, rdi, rdx, rcx, r8, r9
int func(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);

int sum(int a, int b) {
    return a + b;
}

int main(int argc, char *argv[]) {
    int res = func(1, 2, 3, 4, 5, 6, 7, 8);
    printf("res: %d\n", res);
    return 0;
}