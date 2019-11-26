#include<stdio.h>
#include<stdlib.h>

int change_pattern(int value, int nbits, int pattern, int new_pattern) {
    // create a mask of #nbits
    // e.g. if #nbits = 5 -> mask = 0b11111
    int mask = ~(~0 << nbits);

    // we only want the lower #nbits of the new pattern
    // we also assume that the pattern has already #nbits
    new_pattern &= mask;
    // search for the first occurrence of the first #nbits of the pattern
    for (int i = 0; pattern > 0; ++i, pattern <<= 1, mask <<=1) {
        if ((value & mask) == pattern) {
            // set pattern bits to zero in value
            value &= ~mask;
            new_pattern <<= i;
            value |= new_pattern;
            break;
        }
    }

    return value;
}

int main(int argc, char *argv[]) {
    if (argc != 5)
        printf("Usage: ./changepattern [value hex] [nbits] [pattern hex] [new_pattern hex]\n");
    else {
        int value = strtol(argv[1], NULL, 16);
        int nbits = strtol(argv[2], NULL, 10);
        int pattern = strtol(argv[3], NULL, 16);
        int new_pattern = strtol(argv[4], NULL, 16);
        printf("New Value: 0x%08X\n", change_pattern(value, nbits, pattern, new_pattern));
    }
    
    return 0;
}