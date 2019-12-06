#include "libbooks.h"
#include<stdio.h>

typedef size_t (*requestCallback)(char *ptr, size_t size, size_t nmemb, void *userdata);

static const int INITIAL_BUFFER_SIZE = 1024 * 4; // 4kb

typedef struct BufferData {
    size_t currentSize;
    char *jsonBuffer;
} Buffer;

static CURL *curl;

void books_init() {
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Books/1.0");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, TRUE);
    }
}

void books_free() {
    curl_easy_cleanup(curl);
}

int doCurlRequest(const char *uri, void *data, requestCallback callback) {
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, uri);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        CURLcode code = curl_easy_perform(curl);
        return code;
    }

    return 0;
}

size_t jsonRequestCallback(char *ptr, size_t size, size_t nmemb, Buffer *buf) {
    size_t totalSize = size * nmemb;
    char *newBuf = realloc(buf->jsonBuffer, buf->currentSize + totalSize);
    if (newBuf == NULL)
        return 0;

    for (int i = 0; i < totalSize; ++i)
        buf->jsonBuffer[buf->currentSize + i] = ptr[i];

    buf->currentSize += totalSize;
    return totalSize;
}

int httpGetToFile(const char *uri, const char *filename) {
    FILE *file = fopen(filename, "w+");
    if (!file)
        return 0;

    int res = doCurlRequest(uri, file, (requestCallback) fwrite);
    fclose(file);
    return res == CURLE_OK ? 1 : 0;
}

struct json_object *httpGetJsonData(const char *uri) {
    // allocate a json buffer
    char *jsonBuffer = calloc(INITIAL_BUFFER_SIZE, 1);
    if (jsonBuffer == NULL)
        return NULL;

    Buffer buf = { 0, jsonBuffer };
    int err = doCurlRequest(uri, &buf, (requestCallback) jsonRequestCallback);
    if (err != CURLE_OK) {
        fprintf(stderr, "An error occurred: %s\n", curl_easy_strerror(err));
        return NULL;
    }

    json_object *json = json_tokener_parse(jsonBuffer);
    free(jsonBuffer);
    return json;
}

int googleBooksSearchByAuthor(const char *apikey, const char *author, Collection *res) {
    char *encodedAuthor = curl_easy_escape(curl, author, 0);
    char uri[512];
    sprintf(uri, "https://www.googleapis.com/books/v1/volumes?apikey=%s&q=inauthor:%%22%s%%22",
        apikey, encodedAuthor);

    curl_free(encodedAuthor);

    json_object *json = httpGetJsonData(uri);
    puts(json_object_to_json_string_ext(json, 
        JSON_C_TO_STRING_SPACED | 
        JSON_C_TO_STRING_PRETTY_TAB));

    json_object_put(json);
    return -1;
}