#include<stdio.h>
#include<string.h>
#include<errno.h>
#include "libbooks.h"

void test_http_get_to_file() {
    int ret = httpGetToFile("https://api.github.com/users/xploitedd/repos", "google.html");
    if (!ret)
        fprintf(stderr, "An error occurred: %s\n", strerror(errno));
}

int main(int argc, char *argv[]) {
    books_init();
    test_http_get_to_file();
    books_free();
    return 0;
}
