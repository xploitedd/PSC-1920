#ifndef LIBBOOKS_H
#define LIBBOOKS_H

#include<curl/curl.h>
#include<json-c/json.h>
#include<stdbool.h>

const int ERR_PDF_NOT_FOUND = -2;

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
int googleBooksGetUrls(const char *apikey, const char *volumeId,
                       char *thumb_url, size_t thumb_len,
                       char *pdf_url,   size_t pdf_len);
void free_collection(Collection *cl);

#endif
