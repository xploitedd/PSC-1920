#include<stdio.h>
#include<string.h>
#include<errno.h>
#include "libbooks.h"


int testHttpGetToFile(const char *uri, const char *filename){
     int result = httpGetToFile(uri, filename);
     if(result == 1) {
         puts("Success\n");
         return 1;
    } else {
        puts ("Failed to GET\n");
        return 0;
    }
}

void testHttpGetJsonData(char* uri){
    books_init();
    json_object *received = httpGetJsonData(uri);
    if (received == NULL) {
            json_object_put(received);
            return;
    }else {
    printf("Recived Json Object!\n");
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

int main(int argc, char *argv[]){
    printf("httpGetToFile:");
    testHttpGetToFile("https://api.github.com/users/xploitedd/repos", "google.html");
    testHttpGetJsonData("https://www.googleapis.com/books/v1/volumes?q=inauthor:%22Alexandre%20Herculano%22");
}
