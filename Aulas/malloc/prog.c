#include "xmalloc.h"
#include<stdio.h>
#include<string.h>

const char *TEXT = "ISEL - LEIC - PSC";

int main(int argc, char *argv[]) {
    size_t textLen = strlen(TEXT);
    for ( ; ; ) {
        char *copiedText = (char *) xmalloc(textLen + 1);
        if (copiedText != NULL) {
            strcpy(copiedText, TEXT);
            puts(copiedText);
            xfree(copiedText);
        } else {
            printf("Out of Memory\n");
            break;
        }
    }
    return 0;
}