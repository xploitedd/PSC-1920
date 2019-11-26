#include<stdio.h>
#include<stdbool.h>
#include<string.h>

int find_and_copy(
    void *dst, 
    size_t dim, 
    void *ptrs[], 
    size_t n, 
    void *ctx, 
    int (*eval)(void *ptr, void *ctx), 
    bool backwards
);

int find_and_copy_C(
    void *dst, 
    size_t dim, 
    void *ptrs[], 
    size_t n, 
    void *ctx, 
    int (*eval)(void *ptr, void *ctx), 
    bool backwards
) {
    // do only one loop to achieve code simplicity
    int idx = -1;
    for (int i = 0; i < n; ++i) {
        if (eval(ptrs[i], ctx)) {
            idx = i;
            if (!backwards) break;
        }
    }

    if (idx != -1) memcpy(dst, ptrs[idx], dim);
    return idx;
}

int eval(void *ptr, void *ctx) { 
    if (strstr(ptr, ctx) != NULL)
        return 1;
    else
        return 0;
}

void find_and_copy_test() {
    // execute the tests ...
    char *phrases[] = { "Hello World!", "ISEL - PSC - LEIC", "Teste PSC", "World Hello!" };
    char a[8];
    if (find_and_copy(a, 8, (void **) phrases, 4, "World", eval, true) == 3)
        printf("TEST 1 PASSED!\t%s\n", a);

    char b[8];
    if (find_and_copy(b, 8, (void **) phrases, 4, "Hello ", eval, true) == 0)
        printf("TEST 2 PASSED!\t%s\n", b);

    char c[8];
    if (find_and_copy(c, 8, (void **) phrases, 4, "PSC", eval, true) == 2)
        printf("TEST 3 PASSED!\t%s\n", c);

    char d[8];
    if (find_and_copy(d, 8, (void **) phrases, 4, "PSC", eval, false) == 1)
        printf("TEST 4 PASSED!\t%s\n", d);

    char e[8];
    if (find_and_copy(e, 8, (void **) phrases, 4, "ABC", eval, false) == -1)
        printf("TEST 5 PASSED!\t%s\n", e);
}

int main(int argc, char *argv[]) {
    find_and_copy_test();
    return 0;
}