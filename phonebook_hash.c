#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "phonebook_hash.h"

/* hash version */
unsigned int bkdrHash(char *str)
{
    unsigned int seed = 131;
    unsigned int hash = 0;
    while (*str) {
        hash = hash * seed + (*str++);
    }
    return (hash % HASHTABLE_SIZE);
}

hashTable *initHashTable()
{
    hashTable *newHashTable = NULL;
    newHashTable = (hashTable *) malloc(sizeof(hashTable));
    newHashTable->list = (entry **) malloc(sizeof(entry*)*HASHTABLE_SIZE);
    for(int i=0; i<HASHTABLE_SIZE; i++)
        newHashTable->list[i] = NULL;

    return newHashTable;
}

entry *findName(char lastName[], hashTable *ht)
{
    entry *e;
    unsigned int index = bkdrHash(lastName);
    for(e = ht->list[index]; e; e = e->pNext) {
        if(!strcasecmp(lastName, e->lastName))
            return e;
    }
    return NULL;
}

void append(char lastName[], hashTable *hashTable)
{
    /* allocate memory for the new entry and put lastName */
    unsigned int index = bkdrHash(lastName);
    entry *newEntry;
    newEntry = (entry *) malloc(sizeof(entry));
    strcpy(newEntry->lastName, lastName);
    newEntry->pNext = hashTable->list[index];
    hashTable->list[index] = newEntry;
}
