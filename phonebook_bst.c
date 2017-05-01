#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "phonebook_bst.h"

node *findName(char lastName[], node *root)
{
    int val;
    node *tmp = root;
    while (tmp) {
        if (!(val = strcasecmp(lastName, tmp->pEntry->lastName)))
            return tmp;
        else if (val < 0)
            tmp = tmp->left;
        else
            tmp = tmp->right;
    }
    return NULL;
}


entry *append(char lastName[], entry *e)
{
    e->pNext = (entry *) malloc(sizeof(entry));
    e = e->pNext;
    strcpy(e->lastName, lastName);
    e->pNext = NULL;

    return e;
}

node *sortedListToBST (entry *e)
{
    int countListLength = 0;
    entry *tmp = e;
    while (tmp) {
        countListLength++;
        tmp = tmp->pNext;
    }
    return sortedListToBSTRecur(&e, countListLength);
}

node *sortedListToBSTRecur(entry **e, int listlength)
{
    if (listlength <= 0)
        return NULL;
    node *left = sortedListToBSTRecur(e, listlength/2);
    node *root = (node *) malloc(sizeof(node));
    root->left = left;
    root->pEntry = *e;
    *e = (*e)->pNext;
    root->right = sortedListToBSTRecur(e, listlength - listlength/2 - 1);
    return root;
}
