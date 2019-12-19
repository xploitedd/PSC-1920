#include "libbooks.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef size_t (*requestCallback)(char *ptr, size_t size, size_t nmemb, void *userdata);

static const size_t INITIAL_BUFFER_SIZE = 1024 * 4; // 4kb

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

/**
 * do_curl_request
 * request web resource using the curl library
 * @param uri pointer to the resource uri
 * @param data pointer to a data buffer
 * @param callback pointer to the callback function
 * @return true if no error occurred
 */
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

/**
 * json_request_callback
 * handles a curl json response
 */
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

/**
 * httpGetToFile
 * saves the contents of the web resource to a file
 * @param uri pointer to the resource uri
 * @param filename name of the file to save the response to
 * @return true if no error occurred
 */
int httpGetToFile(const char *uri, const char *filename) {
    FILE *file = fopen(filename, "w+");
    if (!file)
        return 0;

    int res = do_curl_request(uri, file, (requestCallback) fwrite);
    fclose(file);
    return res == CURLE_OK ? 1 : 0;
}

/**
 * httpGetJsonData
 * returns a json_object containing the data of the response
 * @param uri pointer to the resource uri
 * @return json_object if successful, NULL pointer otherwise
 */
struct json_object *httpGetJsonData(const char *uri) {
    // allocate a json buffer
    char *jsonBuffer = calloc(INITIAL_BUFFER_SIZE, 1);
    if (jsonBuffer == NULL)
        return NULL;

    Buffer buf = { 0, jsonBuffer };
    // do the request
    int err = do_curl_request(uri, &buf, (requestCallback) json_request_callback);
    if (err != CURLE_OK) {
        fprintf(stderr, "An error occurred: %s\n", curl_easy_strerror(err));
        return NULL;
    }

    // parse the request if successful
    json_object *json = json_tokener_parse(buf.jsonBuffer);
    if (json == NULL) {
        fprintf(stderr, "Invalid json response!\n");
        return NULL;
    }

    free(buf.jsonBuffer);
    return json;
}

/**
 * googleBooksSearchByAuthor
 * search google books by an author name and put the results in the
 * collection passed
 * @param apikey api key for google books
 * @param author name of the author to search for
 * @param res collection where to put the response in
 * @return number of collections if successful, negative otherwise
 */
int googleBooksSearchByAuthor(const char *apikey, const char *author, Collection *res) {
    if (res == NULL) return -1;
    char *encodedAuthor = curl_easy_escape(curl, author, 0);
    for (int i = 0; ; ) {
        char uri[512];
        sprintf(uri, "https://www.googleapis.com/books/v1/volumes?fields=totalItems,items(id,volumeInfo,accessInfo)&filter=full&maxResults=40&key=%s&q=inauthor:%%22%s%%22&startIndex=%d",
            apikey, encodedAuthor, i);
            
        json_object *json = httpGetJsonData(uri);
        if (json == NULL)
            return -1;

        if (res->volumes == NULL) {
             // get the volume count
            json_object *totalItemsJson;
            json_object_object_get_ex(json, "totalItems", &totalItemsJson);
            int totalItems = json_object_get_int(totalItemsJson);
            // put the volume information into the collection
            res->volume_count = totalItems;
            res->volumes = calloc(totalItems, sizeof(Volume));
        }

        // check if we have all the results
        if (i >= res->volume_count) {
            json_object_put(json);
            break;
        }
        
        json_object *items;
        json_object_object_get_ex(json, "items", &items);
        size_t offset = json_object_array_length(items);

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

            // need to request memory blocks because json-c will release upon
            // json_object_put call
            Volume *vol = &res->volumes[i + j];
            const char *volumeId = json_object_get_string(volumeIdObj);
            vol->volumeId = calloc(strlen(volumeId) + 1, 1);
            strcpy(vol->volumeId, volumeId);

            const char *title = json_object_get_string(titleObj);
            vol->title = calloc(strlen(title) + 1, 1);
            strcpy(vol->title, title);

            const char *identifier = json_object_get_string(identifierObj);
            vol->isbn = calloc(strlen(identifier) + 1, 1);
            strcpy(vol->isbn, identifier);

            const char *publishedDate = json_object_get_string(publishedDateObj);
            vol->publishedDate = calloc(strlen(publishedDate) + 1, 1);
            strcpy(vol->publishedDate, publishedDate);

            vol->pdfAvailable = json_object_get_boolean(pdfAvailableObj);
        }
        
        json_object_put(json);
        i += offset;
    }

    curl_free(encodedAuthor);
    return res->volume_count;
}

/**
 * googleBooksGetUrls
 * obtain the thumnail and pdf urls of the specified book if
 * they exist
 * @param apikey google books api key
 * @param volumeId volume to get the urls for
 * @param thumb_url pointer where to copy the thumbnail url to
 * @param thumb_len max length of the thumbnail url
 * @param pdf_url pointer where to copy the pdf url to
 * @param pdf_len max length of the pdf url
 * @return zero if no errors, otherwise returns the error code
 */
int googleBooksGetUrls(const char *apikey, const char *volumeId,
                       char *thumb_url, size_t thumb_len,
                       char *pdf_url,   size_t pdf_len)
{
    char uri[512];
    sprintf(uri, "https://www.googleapis.com/books/v1/volumes/%s?key=%s&projection=lite&fields=volumeInfo(imageLinks),accessInfo(pdf)",
        volumeId, apikey);

    json_object *json = httpGetJsonData(uri);
    if (json == NULL)
        return -1;

    // get thumbnail url
    json_object *volumeInfo;
    json_object_object_get_ex(json, "volumeInfo", &volumeInfo);
    json_object *imageLinks;
    json_object_object_get_ex(volumeInfo, "imageLinks", &imageLinks);
    json_object *thumbnail;
    json_object_object_get_ex(imageLinks, "thumbnail", &thumbnail);
    strncpy(thumb_url, json_object_get_string(thumbnail), thumb_len);

    json_object *accessInfo;
    json_object_object_get_ex(json, "accessInfo", &accessInfo);
    json_object *pdf;
    json_object_object_get_ex(accessInfo, "pdf", &pdf);
    json_object *downloadLink;
    if (!json_object_object_get_ex(pdf, "downloadLink", &downloadLink)) {
        json_object_put(json);
        return ERR_PDF_NOT_FOUND;
    }

    strncpy(pdf_url, json_object_get_string(downloadLink), pdf_len);
    json_object_put(json);
    return 0;
}

/**
 * free_collection
 * free a collection struct
 * @param cl pointer of the struct to be freed
 */
void free_collection(Collection *cl) {
    if (cl != NULL) {
        for (int i = 0; i < cl->volume_count; ++i) {
            Volume *vol = &cl->volumes[i];
            free(vol->volumeId);
            free(vol->title);
            free(vol->isbn);
            free(vol->publishedDate);
        }

        free(cl->volumes);
    }
}