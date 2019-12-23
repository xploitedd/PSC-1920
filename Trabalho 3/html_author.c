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
    

    char fileName[authorSize];
    char authorName[authorSize + 5];
    memset(authorName, 0, authorSize);
    memset(fileName, 0, authorSize + 5);

    size_t current = 0;
    for (int i = 1; i < argc; ++i) {
        char *str = argv[i];
        strcat(authorName, str);
        strcat(fileName, str);
        current += strlen(argv[i]) + 1;
        authorName[current - 1] = ' ';
        fileName[current - 1] = '-';
    }

    authorName[authorSize - 1] = 0;
    strcpy(fileName + (authorSize - 1), ".html\0");

    printf("Author Name: %s\n", authorName);
    printf("File name: %s\n", fileName);

    char *apikey = getenv("GBOOKS_API_KEY");
    if (apikey == NULL) {
        fprintf(stderr, "Error: please provide the api key using the environment variable: GBOOKS_API_KEY\n");
        return -1;
    }

    // get template contents
    FILE *file = fopen(fileName, "w+");
    if (file == NULL) {
        fprintf(stderr, "Error opening file");
        return -1;
    }

    // add author name to the file
    fprintf(
        file, 
        "<!doctype html><html><head><title>%s</title><link href=\"https://unpkg.com/tailwindcss@^1.0/dist/tailwind.min.css\" rel=\"stylesheet\"></head><body><div class=\"container mx-auto my-6\"><h1 class=\"font-bold text-5xl\">%s</h1>", 
        authorName, authorName
    );

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

            fprintf(
                file, 
                "<div class=\"md:flex my-4\"><div class=\"md:flex-shrink-0\"><img class=\"rounded-lg md:w-56\" src=\"%s\"></div><div class=\"mt-4 md:mt-0 md:ml-6\"><a href=\"#\" class=\"block mt-1 text-lg leading-tight font-semibold text-gray-900 hover:underline\">%s</a><p class=\"mt-2 text-gray-600\">Volume ID: %s</p><p class=\"mt-2 text-gray-600\">Identifier: %s</p><p class=\"mt-2 text-gray-600\">Published Date: %s</p>", 
                thumb_url, vol.title, vol.volumeId, vol.identifier, vol.publishedDate);

            if (vol.pdfAvailable)
                fprintf(file, "<a href=\"%s\" class=\"mt-2\">PDF Download Link</a><br>", pdf_url);

            if (vol.epubAvailable)
                fprintf(file, "<a href=\"%s\" class=\"mt-2\">EPUB Download Link</a>", epub_url);

            fprintf(file, "</div></div>");
            free(pdf_url);
            free(epub_url);
            free(thumb_url);
        }
    }

    fprintf(file, "</div></body></html>");
    free(file);
    free_collection(&res);
    books_free();
    return 0;
}
