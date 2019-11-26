#include<stdio.h>
#include<string.h>

int numbers[] = { 7, -21, 35, -99, 0, -1, 23, 77, 201, -3 };
const char *text[] = { "ISEL", "LEIC", "PSC" };

const void *xfind(
    const void *items, 
    size_t item_size, 
    size_t count, 
    const void *ref,
    int (*comp)(const void *item, const void *ref))
{
    for (int i = 0; i < count; ++i, items += item_size) {
        if (comp(items, ref))
            return items;
    }

    return NULL;
}

int int_compare_less(const void *item, const void *ref) {
    int first = *((const int *) item);
    int other = *((const int *) ref);
    return first < other;
}

int str_compare_less(const void *item, const void *ref) {
    const char *txt1 = *((const char **) item);
    const char *txt2 = *((const char **) ref);
    return strcmp(txt1, txt2) < 0;
}

int main(int argc, char *argv[]) {
    int refInt = 50;
    const char *refStr = "K";
    const int *resInt = xfind(numbers, sizeof(int), 10, &refInt, int_compare_less);
    const char * const *resStr = xfind(text, sizeof(const char *), 3, &refStr, str_compare_less);
    printf("resInt: %d\n", *resInt);
    printf("resStr: %s\n", *resStr);
    return 0;
}