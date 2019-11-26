#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<inttypes.h>

#define FLOATING_POINT_DIVIDER ','
#define EXPONENT_BIT_CONSTANT 127
#define INT_HIGH 0x40000000
#define MANTISSA_MAX_BITS 23
#define MANTISSA_BITS 0x7FFFFF
#define MANTISSA_HIGHER_BIT_VALUE 0x400000

struct ftype {
    unsigned m : 23;
    unsigned e : 8;
    unsigned s : 1;
};

union number {
    float f;
    struct ftype bits;
};

float string_to_float(char *string) {
    // find the comma
    int comma = 0, len = strlen(string);
    for (; comma < len && string[comma] != FLOATING_POINT_DIVIDER; ++comma);
    uint8_t is_negative = string[0] == '-';
    // convert the integer and decimal parts to int
    unsigned int integer_part = 0, decimal_part = 0;
    for (int i = is_negative; i < comma; ++i) {
        char digit = string[i];
        if (digit < '0' || digit > '9')
            return 0;

        integer_part *= 10;
        integer_part += digit - '0';
    }

    int mantissa_sbb = 1;
    for (int i = comma + 1; i < len; ++i) {
        char digit = string[i];
        if (digit < '0' || digit > '9')
            return 0;

        mantissa_sbb *= 10;
        decimal_part *= 10;
        decimal_part += digit - '0';
    }

    // optimization
    if (integer_part == 0 && decimal_part == 0)
        return 0;

    // calculate the mantissa
    uint32_t mantissa = 0;
    for (int i = 0; i < MANTISSA_MAX_BITS; ++i) {
        mantissa <<= 1;
        decimal_part *= 2;
        if (decimal_part >= mantissa_sbb) {
            decimal_part -= mantissa_sbb;
            mantissa |= 1;
        }
    }

    int exp = 0, error = 0;
    // find the exponent and the definitive mantissa
    if (integer_part == 0) {
        // means that the decimal_part != 0, therefore the exponent is negative
        exp = -1;
        for (int i = MANTISSA_HIGHER_BIT_VALUE; !(i & mantissa); i >>= 1, --exp);
        // generate #exponent new bits for the mantissa and discard the higher
        for (int i = 0; i < -exp + 1; ++i) {
            mantissa <<= 1;
            decimal_part *= 2;
            if (decimal_part >= mantissa_sbb) {
                decimal_part -= mantissa_sbb;
                mantissa |= 1;
            }
        }

        error = mantissa & 1;
        mantissa >>= 1;
        mantissa &= MANTISSA_BITS;
    } else {
        // means that the integer_part != 0, therefore the exponent is positive
        // find the most significant one
        int i = INT_HIGH;
        for (; !(i & integer_part); i >>= 1);
        for (int j = 1; j < i; j *= 2, ++exp);
        // only need to affect the mantissa if the exponent is greater than zero
        if (exp > 0) {
            // cut the MSO off and insert the remaining bits into the mantissa
            mantissa >>= exp - 1;
            // save the error bit, used to round the number
            error = mantissa & 1;
            mantissa >>= 1;
            mantissa |= ((integer_part & ~i) << (MANTISSA_MAX_BITS - exp));
        }
    }

    // IEEE 754 (single point precision): 
    // [sign 1-bit][exponent 8-bits][mantissa 23-bits]
    union number num;
    num.bits.s = is_negative;
    num.bits.e = EXPONENT_BIT_CONSTANT + exp;
    num.bits.m = mantissa + error;
    return num.f;
}

int main(int argc, char *argv[]) {
    if (argc < 2)
        printf("please insert a value\ne.g. ./stringtofloat 23,23\n");
    else
        printf("Value in float: %f\n", string_to_float(argv[1]));
    
    return 0;
}  