#include<stdio.h>
#include<inttypes.h>
#include<stdlib.h>
#include<string.h>

struct node {
    struct node *next;
};

struct word {
    struct node node;
    char *word;
};

struct node *list1 = NULL;

int compword(struct node *node, const void *data) {
    struct word *pwc = (struct word *) node;
    const char *word = (const char *) data;
    return strcmp(pwc->word, word);
}

void insertNode(struct node **list, struct node *node) {
    node->next = *list;
    *list = node;
}

struct node *findNode(
    struct node *list, 
    int (*comp)(struct node *node, const void *data), 
    const void *pdata) 
{
    struct node *cur;
    for (cur = list; cur && comp(cur, pdata); cur = cur->next);
    return cur;
}

int main(int argc, char *argv[]) {
    struct word w1;
    w1.word = "abc";
    struct word w2;
    w2.word = "xyz";
    insertNode(&list1, &(w1.node));
    struct word *w = (struct word *) findNode(list1, compword, w1.word);
    printf("addr1: %p\tvalue1: %s\n", w, w->word);
    insertNode(&list1, &(w2.node));
    w = (struct word *) findNode(list1, compword, w1.word);
    printf("addr1: %p\tvalue1: %s\n", w, w->word);
    w = (struct word *) findNode(list1, compword, w2.word);
    printf("addr2: %p\tvalue2: %s\n", w, w->word);
    printf("list 1 address: %p\n", list1);
    return 0;
}