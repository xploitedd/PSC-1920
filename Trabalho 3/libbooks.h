#ifndef LIBBOOKS_H
#define LIBBOOKS_H

#include<curl/curl.h>
#include<json-c/json.h>
#include<stdbool.h>

typedef struct volume {
    char *volumeId;
    char *title;
    char *isbn;
    char *publishedDate;
    bool pdfAvailable;
    bool epubAvailable;
} Volume;

typedef struct collection {
    size_t volume_count;
    Volume *volumes;
} Collection;

void books_init();
void books_free();

int httpGetToFile(const char *uri, const char *filename);
struct json_object *httpGetJsonData(const char *uri);
int googleBooksSearchByAuthor(const char *apikey, const char *author, Collection *res);
void free_collection(Collection *cl);

#endif
