#include "libbooks.h"
#include<stdio.h>
#include<string.h>

#define TIMEOUT 5

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Error: please provide an author name!\nSyntax: %s <author_name>\n", argv[0]);
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

    authorName[authorSize - 1] = 0;
    printf("Author Name: %s\n", authorName);

    char *apikey = getenv("GBOOKS_API_KEY");
    if (apikey == NULL) {
        fprintf(stderr, "Error: please provide the api key using the environment variable: GBOOKS_API_KEY\n");
        return -1;
    }

    FILE *template = fopen("template.html", "r");
    if (template == NULL) {
        fprintf(stderr, "Template file <template.html> not found!");
        return -1;
    }

    books_init();
    Collection res = { 0, NULL };
    int currentAttempts = 0;
    if (googleBooksSearchByAuthor(apikey, authorName, &res) > 0) {
        for (int i = 0; i < res.volume_count; ++i) {
            size_t url_len = 128;
            char *pdf_url = calloc(url_len, 1);
            char *epub_url = calloc(url_len, 1);
            char *thumb_url = calloc(url_len, 1);

            Volume vol = res.volumes[i];
            int err = googleBooksGetUrls(apikey, vol.volumeId, thumb_url, url_len, pdf_url, url_len, epub_url, url_len);
            if (err == -1) {
                free(pdf_url);
                free(thumb_url);
                free(epub_url);

                if (++currentAttempts == TIMEOUT) break;
                continue;
            }

            printf("------------------------------------------------------------------------\n");
            printf("Title:     %s\n", vol.title);
            //printf("PDF Available:  %d\n", vol.pdfAvailable);
            printf("PDF Available:  %d\n", strlen(pdf_url) > 0);
            printf("PDF url:   %s\n", pdf_url);
            printf("EPUB Available:  %d\n", strlen(epub_url) > 0);
            //printf("EPUB Available:  %d\n", vol.epubAvailable);
            printf("EPUB url:   %s\n", epub_url);
            printf("thumb url: %s\n", thumb_url);

            free(pdf_url);
            free(epub_url);
            free(thumb_url);
        }
    }

    fclose(template);
    free_collection(&res);
    books_free();
    return 0;
}
