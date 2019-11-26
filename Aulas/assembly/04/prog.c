#include<stdio.h>
#include<string.h>

int numbers[] = { 7, -21, 35, -99, 0, -1, 23, 77, 201, -3 };
const char *text[] = { "ISEL", "LEIC", "PSC" };

int findIntGreaterThan(const int *items, size_t count, int ref) {
    for (int i = 0; i < count; ++i) {
        if (items[i] > ref)
            return items[i];
    }

    return (int) ~(((unsigned int)(~0)) >> 1);
}

const char *findStrShorterThan(const char *str[], size_t count, size_t ref) {
    for (int i = 0; i < count; ++i) {
        int len = strlen(str[i]);
        if (len < ref)
            return str[i];
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    int resInt = findIntGreaterThan(numbers, 10, 50);
    const char *resStr = findStrShorterThan(text, 3, 4);
    printf("resInt: %d\n", resInt);
    printf("resStr: %s\n", resStr);
    return 0;
}