#include <stdio.h>
#include <stdlib.h>

struct student {
    int number;
    char name[128];
};

int main(int argc, char *args[]) {
    struct student st = { 46074, "Ricardo Margalhau" };
    int arr[] = { 123, 213, 12323, 312 };
    printf("array base addr:\t %p\n", arr);
    for (int i = 0; i < sizeof(arr)/sizeof(int); ++i)
        printf("element %d address:\t %p\n", i, &arr[i]);

    printf("2nd array value:\t %d\n", *(arr + 1));
    printf("-------------------------------\n");
    printf("base address of struct:\t %p\n", &st);
    printf("address of number:\t %p\n", &(st.number));
    printf("address of name:\t %p\n", &(st.name));
    struct student *st2 = &st;
    printf("address of 2nd student:\t %p\n", st2);
}