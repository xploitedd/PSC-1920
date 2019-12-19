#include "libbooks.h"
#include<stdio.h>
#include<string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Error: please provide an author name!\nExample: %s <author_name>\n", argv[0]);
        return -1;
    }

    size_t authorSize = 0;
    for (int i = 1; i < argc; ++i)
        authorSize += strlen(argv[i]) + 1;

    char authorName[authorSize];
    memset(authorName, 0, authorSize);

    size_t current = 0;
    for (int i = 1; i < argc; ++i) {
        char *str = argv[i];
        strcat(authorName, str);
        current += strlen(argv[i]) + 1;
        authorName[current - 1] = ' ';
    }

    printf("Author Name: %s\n", authorName);

    char *apikey = getenv("GBOOKS_API_KEY");
    if (apikey == NULL) {
        fprintf(stderr, "Error: please provide the api key using the environment variable: GBOOKS_API_KEY\n");
        return -1;
    }

    books_init();
    Collection res = { 0, NULL };
    if (googleBooksSearchByAuthor(apikey, authorName, &res) > 0) {
        printf("count: %ld\n", res.volume_count);
        for (int i = 0; i < res.volume_count; ++i) {
            size_t url_len = 128;
            char *pdf_url = calloc(url_len, 1);
            char *thumb_url = calloc(url_len, 1);

            int err = googleBooksGetUrls(apikey, res.volumes[i].volumeId, thumb_url, url_len, pdf_url, url_len);
            if (err == -1) continue;

            printf("------------------------------------------------------------------------\n");
            printf("title:     %s\n", res.volumes[i].title);
            printf("pdf url:   %s\n", pdf_url);
            printf("thumb url: %s\n", thumb_url);

            free(pdf_url);
            free(thumb_url);
        }
    }

    free_collection(&res);
    books_free();
    return 0;
}