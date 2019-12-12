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
    int err = googleBooksSearchByAuthor("AIzaSyDQcIpcRauamoUdu0s9AYKSyPJX7VjAfr8", "Edgar Allan Poe", &col);
    printf("Error ? %d Count: %ld\n", err, col.volume_count);
    for (int i = 0; i < col.volume_count; ++i) {
        Volume vol = col.volumes[i];
        printf("------------------------------------\n");
        printf("Idx:            %d\n", i);
        printf("Id:             %s\n", vol.volumeId);
        printf("Title:          %s\n", vol.title);
        printf("Published Date: %s\n", vol.publishedDate);
        printf("ISBN:           %s\n", vol.isbn);
        printf("PDF Available:  %d\n", vol.pdfAvailable);
    }

    free_collection(&col);
    books_free();
    return 0;
}