#include "libbooks.h"
#include<stdio.h>
#include<string.h>

typedef size_t (*requestCallback)(char *ptr, size_t size, size_t nmemb, void *userdata);

static const int INITIAL_BUFFER_SIZE = 1024 * 4; // 4kb

typedef struct BufferData {
    size_t currentSize;
    char *jsonBuffer;
} Buffer;

static CURL *curl;

/**
 * books_init
 * initializes the library and needed resources
 */
void books_init() {
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Books/1.0");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, TRUE);
    }
}

/**
 * books_free
 * free the resources used by the library
 */
void books_free() {
    curl_easy_cleanup(curl);
}

int do_curl_request(const char *uri, void *data, requestCallback callback) {
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, uri);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        CURLcode code = curl_easy_perform(curl);
        return code;
    }

    return 0;
}

size_t json_request_callback(char *ptr, size_t size, size_t nmemb, Buffer *buf) {
    size_t totalSize = size * nmemb;
    char *newBuf = realloc(buf->jsonBuffer, buf->currentSize + totalSize + 1);
    if (newBuf == NULL)
        return 0;

    buf->jsonBuffer = newBuf;
    memcpy(&(buf->jsonBuffer[buf->currentSize]), ptr, totalSize);
    buf->currentSize += totalSize;
    buf->jsonBuffer[buf->currentSize] = 0;
    return totalSize;
}

int httpGetToFile(const char *uri, const char *filename) {
    FILE *file = fopen(filename, "w+");
    if (!file)
        return 0;

    int res = do_curl_request(uri, file, (requestCallback) fwrite);
    fclose(file);
    return res == CURLE_OK ? 1 : 0;
}

struct json_object *httpGetJsonData(const char *uri) {
    // allocate a json buffer
    char *jsonBuffer = calloc(INITIAL_BUFFER_SIZE, 1);
    if (jsonBuffer == NULL)
        return NULL;

    Buffer buf = { 0, jsonBuffer };
    int err = do_curl_request(uri, &buf, (requestCallback) json_request_callback);
    if (err != CURLE_OK) {
        fprintf(stderr, "An error occurred: %s\n", curl_easy_strerror(err));
        return NULL;
    }

    json_object *json = json_tokener_parse(buf.jsonBuffer);
    if (json == NULL) {
        fprintf(stderr, "Invalid json response!");
        return NULL;
    }

    free(buf.jsonBuffer);
    return json;
}

int googleBooksSearchByAuthor(const char *apikey, const char *author, Collection *res) {
    if (res == NULL) return -1;
    char *encodedAuthor = curl_easy_escape(curl, author, 0);
    for (int i = 0; ; ) {
        char uri[512];
        sprintf(uri, "https://www.googleapis.com/books/v1/volumes?filter=full&maxResults=40&apikey=%s&q=inauthor:%%22%s%%22&startIndex=%d",
            apikey, encodedAuthor, i);

        json_object *json = httpGetJsonData(uri);
        if (json == NULL)
            return -1;

        // get the volume count
        // update the volume count each iteration so that if
        // a new book is added while running it works
        json_object *totalItemsJson;
        json_object_object_get_ex(json, "totalItems", &totalItemsJson);
        int totalItems = json_object_get_int(totalItemsJson);
        if (i >= totalItems)
            break;

        if (res->volumes == NULL) {
            // put the volume information into the collection
            res->volume_count = totalItems;
            res->volumes = calloc(totalItems, sizeof(Volume));
        } else if (res->volume_count != totalItems) {
            int dif = totalItems - res->volume_count;
            if (dif > 0) {
                Volume *ptr = realloc(res->volumes, totalItems * sizeof(Volume));
                if (ptr == NULL)
                    return -1;

                res->volume_count = totalItems;
                res->volumes = ptr;
            }
        }
        
        json_object *items;
        json_object_object_get_ex(json, "items", &items);
        size_t offset = json_object_array_length(items);
        i += offset;

        for (int j = 0; j < offset; ++j) {
            json_object *volumeObj = json_object_array_get_idx(items, j);
            json_object *volumeIdObj;
            json_object *titleObj;
            json_object *identifierObj;
            json_object *publishedDateObj;
            json_object *pdfAvailableObj;

            json_object_object_get_ex(volumeObj, "id", &volumeIdObj);

            json_object *volumeInfo;
            json_object_object_get_ex(volumeObj, "volumeInfo", &volumeInfo);
            json_object_object_get_ex(volumeInfo, "title", &titleObj);
            json_object_object_get_ex(volumeInfo, "publishedDate", &publishedDateObj);
            
            json_object *industryIdentifiers;
            json_object_object_get_ex(volumeInfo, "industryIdentifiers", &industryIdentifiers);
            // get the first identifier
            json_object *ii_first = json_object_array_get_idx(industryIdentifiers, 0);
            json_object_object_get_ex(ii_first, "identifier", &identifierObj);

            json_object *accessInfo;
            json_object_object_get_ex(volumeObj, "accessInfo", &accessInfo);
            json_object *pdfObj;
            json_object_object_get_ex(accessInfo, "pdf", &pdfObj);
            json_object_object_get_ex(pdfObj, "isAvailable", &pdfAvailableObj);

            res->volumes[j].volumeId = (char *) json_object_get_string(volumeIdObj);
            res->volumes[j].title = (char *) json_object_get_string(titleObj);
            res->volumes[j].isbn = (char *) json_object_get_string(identifierObj);
            res->volumes[j].publishedDate = (char *) json_object_get_string(publishedDateObj);
            res->volumes[j].pdfAvailable = json_object_get_boolean(pdfAvailableObj);
        }
        
        json_object_put(json);
    }

    curl_free(encodedAuthor);
    return res->volume_count;
}

void free_collection(Collection *cl) {
    if (cl != NULL)
        free(cl->volumes);
}