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
    Collection col = { 0, NULL };
    int s = googleBooksSearchByAuthor("AIzaSyDQcIpcRauamoUdu0s9AYKSyPJX7VjAfr8", "Alexandre Herculano", &col);
    //printf("%s\n", col.volumes[0].title);
    free_collection(&col);
    books_free();
    return 0;
}