#include<stdio.h>
#include "calc.h"
#include<curl/curl.h>
#include<json-c/json.h>

int main(int argc, char *argv[]) {
    int a = 3;
    int b = 4;

    int s = add(a, b);
    int p = mul(ans, b);

    printf("sum:\t%d\n", s);
    printf("mul:\t%d\n", p);
    return 0;
}