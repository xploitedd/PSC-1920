#include<stdio.h>

struct example {
    unsigned int number;
    float rate;
    char text[8];
    short cat;
    int id;
};

int func1();
int func2(int a);
int func3(int a, int b);
int func4(char *str);
int func5(int items[], int idx);
int func6(struct example *ex);

int main(int argc, char *argv[]) {
    printf("res1: %d\n", func1());
    printf("res2: %d\n", func2(5));
    printf("res3: %d\n", func3(2, 3));
    printf("string len: %d\n", func4("abc, xpto!"));
    int items[] = { 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007 };
    printf("func5: %d\n", func5(items, 5));
    struct example ex = { 12, 1.0f, "abcde", 8888, 20 };
    printf("func6: %d\n", func6(&ex));
    return 0;
}