#include<stdio.h>
#include<string.h>
#include<errno.h>
#include "libbooks.h"


void testHttpGetToFile(const char *uri, const char *filename){
    int ret = httpGetToFile(uri, filename);
    if (!ret)
        fprintf(stderr, "An error occurred: %s\n", strerror(errno));
}

void testHttpGetJsonData(char* uri){
    json_object *received = httpGetJsonData(uri);
    if (received == NULL) {
        json_object_put(received);
    } else {
        json_object *totalItemsJson;
        json_object_object_get_ex(received, "totalItems", &totalItemsJson);
        int totalItems = json_object_get_int(totalItemsJson);
        printf("%d\n", totalItems);
        json_object *items;
        if (!json_object_object_get_ex(received, "items", &items))
            json_object_put(received);

        size_t offset = json_object_array_length(items);
        for (int j = 0; j < offset; ++j) {
            json_object *volumeObj = json_object_array_get_idx(items, j);
            json_object *volumeInfo;
            json_object_object_get_ex(volumeObj, "volumeInfo", &volumeInfo);
            json_object *titleObj;
            json_object_object_get_ex(volumeInfo, "title", &titleObj);
            json_object *subtitleObj;
            json_object_object_get_ex(volumeInfo, "subtitle", &subtitleObj);
            printf("Volume %d\n", j);
            printf("Title: %s \n",json_object_get_string(titleObj));
            if(subtitleObj != NULL)
                printf("Subtitle: %s \n",json_object_get_string(subtitleObj));
        }
    }
}

void printVol(Collection col){
    for (int i = 0; i < col.volume_count; ++i) {
        Volume vol = col.volumes[i];
        printf("------------------------------------\n");
        printf("Idx:            %d\n", i);
        printf("Id:             %s\n", vol.volumeId);
        printf("Title:          %s\n", vol.title);
        printf("Published Date: %s\n", vol.publishedDate);
        printf("ISBN:           %s\n", vol.identifier);
        printf("PDF Available:  %d\n", vol.pdfAvailable);
        printf("EPUB Available:  %d\n", vol.epubAvailable);
    }
}

void testGoogleBooksSearchByAuthor(char *apikey) {
    Collection col = { 0, NULL };
    Collection col1 = { 0, NULL };

    int books1 = googleBooksSearchByAuthor(apikey, "Camilo Castelo Branco", &col);
    int books2 = googleBooksSearchByAuthor(apikey, "Alexandre Herculano", &col1);
    if (books1 >= 0)
        printVol(col);

    if (books2 >= 0)
        printVol(col1);

    free_collection(&col);
    free_collection(&col1);
}


void testGoogleBooksGetUrls(char *apikey) {
    char *pdf = calloc(100, 1); 
    char *epub = calloc(100, 1);
    char *thumbnail = calloc(100, 1);

    Collection col = { 0, NULL };
    Collection col1 = { 0, NULL };

    int books1 = googleBooksSearchByAuthor(apikey, "Camilo Castelo Branco", &col);
    int books2 = googleBooksSearchByAuthor(apikey, "Alexandre Herculano", &col1);
    if (books1 < 0 || books2 < 0) return;

    for (int i = 0; i < col.volume_count; ++i) {
        Volume vol = col.volumes[i];

        printf("Book Name: %s\n", vol.title);
        int errUrl = googleBooksGetUrls(apikey, vol.volumeId, thumbnail, 100, pdf, 100,epub,100);
        if (errUrl != -1) {
            if(vol.pdfAvailable)
                printf("pdf url: %s thumbnail: %s\n", pdf, thumbnail);
        }
    }

    for (int i = 0; i < col1.volume_count; ++i) {
        Volume vol = col.volumes[i];
        printf("Book Name: %s\n", vol.title);
        int errUrl1 = googleBooksGetUrls(apikey, vol.volumeId, thumbnail, 100, pdf, 100,epub,100);
        if (errUrl1 != -1) {
            if(vol.pdfAvailable)
                printf("pdf url: %s thumbnail: %s\n", pdf, thumbnail);
        }
    }

    free(epub);
    free(pdf);
    free(thumbnail);
    free_collection(&col);
    free_collection(&col1);
    fprintf(stderr, "Error: %d\n", errno);
}

int main(int argc, char *argv[]) {
    books_init();
    char *apikey = getenv("GBOOKS_API_KEY");
    if (apikey == NULL) {
        fprintf(stderr, "Error: please provide the api key using the environment variable: GBOOKS_API_KEY\n");
        return -1;
    }

    testHttpGetToFile("https://www.google.com", "google.html");
    
    printf("HttpGetJsonData:\n");
    testHttpGetJsonData("https://www.googleapis.com/books/v1/volumes?q=inauthor:%22Alexandre%20Herculano%22");
    
    printf("googleBooksSearchByAuthor:\n");
    testGoogleBooksSearchByAuthor(apikey);
    printf("testGoogleBooksGetUrls:\n");
    testGoogleBooksGetUrls(apikey);
    books_free();
}
