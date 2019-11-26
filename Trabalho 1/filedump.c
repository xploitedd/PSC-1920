#include<stdio.h>
#include<string.h>
#include<inttypes.h>
#include<errno.h>
#include<stdlib.h>

#define MAX_FILENAME_LENGTH 256
#define BYTES_PER_LINE 16

uint8_t is_canonical = 0;
uint8_t is_two_bytes = 0;
uint8_t two_bytes_after = 0;

void print_bytes(uint8_t *buffer, size_t count, int two_bytes) {
    for (int j = 0; j < BYTES_PER_LINE; ++j) {
        if (j % 2 == 1) {
            // check if j < count for alignment
            if (j < count) {
                // if two_bytes is true then print the bytes grouped
                // in two (little-endian)
                if (two_bytes)
                    printf("%02x%02x", buffer[j], buffer[j - 1]);
                else
                    printf("%02x %02x", buffer[j - 1], buffer[j]);
            } else {
                // if it is the last line and has no sufficient
                // bytes to fill it, then fill it spaces
                // (for visual effect)
                printf("  ");
                if (!two_bytes)
                    printf(" ");

                printf("  ");
            }
        } else if (j != 0) {
            printf("%c", two_bytes ? '\t' : ' ');
        }
    }
}

int handle_file(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening the file: %s\nFilename:%s\n", strerror(errno), filename);
        return errno;
    }

    // get file size
    size_t fsize = 0;
    fseek(file, 0L, SEEK_END);
    fsize = ftell(file);
    // because we use seek we need to set our cursor back on the beggining
    rewind(file);
    // calculate how many lines we need
    int lines = (fsize / BYTES_PER_LINE) + 1;
    // read the bytes using a buffer
    uint8_t buffer[BYTES_PER_LINE];
    int next_line_idx = 0;
    for (int i = 0; i < lines; ++i) {
        size_t count = 0;
        // read the line bytes to a BYTES_PER_LINE-buffer
        for (int j = 0; j < BYTES_PER_LINE; ++j) {
            uint8_t nb = fgetc(file);
            if (feof(file)) {
                break;
            } else {
                ++count;
                buffer[j] = nb;
            }
        }

        // print last line byte counter
        printf("%08x ", next_line_idx);
        // print the bytes by the order it was specificated with -C and -x
        print_bytes(buffer, count, is_two_bytes && !two_bytes_after);
        if (is_canonical) {
            // print a canonical section each line after
            // the bytes
            printf("  |");
            for (int j = 0; j < count; ++j) {
                uint8_t c = buffer[j];
                if (c == '\n')
                    printf(".");
                else
                    printf("%c", buffer[j]);
            }

            printf("|");
            if (is_two_bytes) {
                printf("\n         ");
                print_bytes(buffer, count, two_bytes_after);
            }
        }

        next_line_idx += count;
        printf("\n");
    }

    fclose(file);
    printf("%08X\n", next_line_idx);

    return 0;
}

int handle_arguments(int argc, char *argv[]) {
    int filenameIdx = 0;
    // search for the -C and -x arguments from the filename index until the end
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-C")) {
            is_canonical = 1;
        } else if (!strcmp(argv[i], "-x")) {
            is_two_bytes = 1;
            two_bytes_after = is_canonical;
        } else {
            filenameIdx = i;
        }
    }

    if (!is_canonical)
        is_two_bytes = 1;

    // return the index where is the filename (last unused argument)
    return filenameIdx;
}

int main(int argc, char *argv[]) {
    char filename[MAX_FILENAME_LENGTH];
    int filenameIdx = handle_arguments(argc, argv);
    if (!filenameIdx) {
        // no argument was passed
        is_two_bytes = 1;
        printf("Please enter the name of the file: ");
        // if the file is passed as program input then only
        // accept a maximum of 256 bytes
        fgets(filename, MAX_FILENAME_LENGTH, stdin);
        // replace the trailing \n with a \0
        for (int i = 0; i < MAX_FILENAME_LENGTH; ++i) {
            if (filename[i] == '\n')
                filename[i] = '\0';
        }
    } else {
        strcpy(filename, argv[filenameIdx]);
    }

    return handle_file(filename);
}