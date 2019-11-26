#include<stdio.h>

void rotate_right(unsigned long value[], size_t n);

int main(int argc, char *argv[]) {
    unsigned long value[] = { 0x31, 0x62 };
    for (int i = 0; i < 2; ++i) {
        printf("%d: %016lX\n", i, value[i]);
    }
    rotate_right(value, 4);
    for (int i = 0; i < 2; ++i) {
        printf("%d: %016lX\n", i, value[i]);
    }
    return 0;
}