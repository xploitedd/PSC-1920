#include<stdio.h>
#include<inttypes.h>
#include<stdlib.h>
#include<string.h>

struct node {
    struct node *left;
    struct node *right;
};

struct word {
    struct node node;
    char *word;
};

struct node *tree1 = NULL;

int compword(struct node **node, const void *data) {
    struct word *pwc = (struct word *) *node;
    char *word = (char *) data;
    return strcmp(pwc->word, word);
}

struct node **findNode(
    struct node **tree, 
    int (*comp)(struct node **node, const void *data), 
    const void *pdata) 
{
    if (*tree == NULL)
        return tree;

    int cmp = comp(tree, pdata);
    if (!cmp)
        return tree;
    else if (cmp < 0)
        return findNode(&((*tree)->left), comp, pdata);

    return findNode(&((*tree)->right), comp, pdata);
}

struct node *insertNode(struct node **tree, struct node *node) {
    struct word *word = (struct word *) node;
    struct node **cur = findNode(tree, compword, word->word);
    *cur = node;
    return *cur;
}

int main(int argc, char *argv[]) {
    struct word w1;
    w1.word = "abc";
    //strcpy(w1.word, "abc");
    struct word w2;
    w2.word = "xyz";
    //strcpy(w2.word, "xyz");
    struct word *wt1 = (struct word *) insertNode(&tree1, &(w1.node));
    printf("node inserted addr: %p, value: %s\n", wt1, wt1->word);
    struct word *wt2 = (struct word *) insertNode(&tree1, &(w2.node));
    printf("node inserted addr: %p, value: %s\n", wt2, wt2->word);
    return 0;
}