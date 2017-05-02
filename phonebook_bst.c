#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "phonebook_bst.h"

entry *findName(char lastName[], node *root)
{
    int count;
    node *result = findGroupEntryNode(lastName, root);
    if (result) {
        count = -1;
        while (++count < GROUP_ENTRY_NUM) {
            if (!strcasecmp(lastName, result->pEntry[count]->lastName)) return result->pEntry[count];
        }
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

node *findGroupEntryNode(char lastName[], node *root)
{
    while (root) {
        int val, node_num = GROUP_ENTRY_NUM;
        do {
            if (root->pEntry[node_num--]) {
                if (!(val = strcasecmp(lastName, root->pEntry[node_num]->lastName)))
                    return root;
            }
        } while (node_num);
        if (val < 0)
            root = root->left;
        else
            root = root->right;
    }
    return NULL;
}


node *sortedListToBST (entry *e)
{
    int countListLength = 0;
    entry *tmp = e;
    while (tmp) {
        countListLength++;
        tmp = tmp->pNext;
    }
    if (countListLength % GROUP_ENTRY_NUM)
        return sortedListToBSTRecur(&e, countListLength / GROUP_ENTRY_NUM + 1);
    return sortedListToBSTRecur(&e, countListLength / GROUP_ENTRY_NUM);

}

node *sortedListToBSTRecur(entry **e, int listlength)
{
    int count = -1;
    if (listlength <= 0)
        return NULL;
    node *left = sortedListToBSTRecur(e, listlength/2);
    node *root = (node *) malloc(sizeof(node));
    root->left = left;
    while (++count < GROUP_ENTRY_NUM) {
        if ((*e)->pNext) {
            root->pEntry[count] = *e;
            *e = (*e)->pNext;
        } else
            root->pEntry[count] = NULL;
    }
    root->right = sortedListToBSTRecur(e, listlength - listlength/2 - 1);
    return root;
}
