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
    books_init();
    json_object *received = httpGetJsonData(uri);
    if (received == NULL) {
            json_object_put(received);
            return;
    }else {
    json_object *totalItemsJson;
    json_object_object_get_ex(received, "totalItems", &totalItemsJson);
    int totalItems = json_object_get_int(totalItemsJson);
    printf("%d\n", totalItems);
    json_object *items;
        if (!json_object_object_get_ex(received, "items", &items)) {
            json_object_put(received);
        }
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
        if(subtitleObj != NULL){printf("Subtitle: %s \n",json_object_get_string(subtitleObj));}
    }
    }
    books_free();
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
void testgoogleBooksSearchByAuthor(){
    books_init();
    Collection col = { 0, NULL };
    Collection col1 = { 0, NULL };
    int err = googleBooksSearchByAuthor("AIzaSyDQcIpcRauamoUdu0s9AYKSyPJX7VjAfr8", "Camilo Castelo Branco", &col);
    int err1 = googleBooksSearchByAuthor("AIzaSyDQcIpcRauamoUdu0s9AYKSyPJX7VjAfr8", "Alexandre Herculano", &col1);
    printVol(col);
    printVol(col1);
    free_collection(&col);
    free_collection(&col1);
    books_free();
}



int main(int argc, char *argv[]){
    printf("httpGetToFile:");
    testHttpGetToFile("https://api.github.com/users/xploitedd/repos", "hello.html");
    testHttpGetJsonData("https://www.googleapis.com/books/v1/volumes?q=inauthor:%22Alexandre%20Herculano%22");
    //testgoogleBooksSearchByAuthor();
}
