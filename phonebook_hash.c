#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "phonebook_hash.h"

/* hash version */

unsigned int SDBMHash(char *str)
{
    unsigned int hash = 0;

    while (*str) {
        // equivalent to: hash = 65599*hash + (*str++);
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }

    return (hash & 0x7FFFFFFF);
}

// RS Hash Function
unsigned int RSHash(char *str)
{
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;

    while (*str) {
        hash = hash * a + (*str++);
        a *= b;
    }

    return (hash & 0x7FFFFFFF);
}

// JS Hash Function
unsigned int JSHash(char *str)
{
    unsigned int hash = 1315423911;

    while (*str) {
        hash ^= ((hash << 5) + (*str++) + (hash >> 2));
    }

    return (hash & 0x7FFFFFFF);
}

// P. J. Weinberger Hash Function
unsigned int PJWHash(char *str)
{
    unsigned int BitsInUnignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    unsigned int ThreeQuarters    = (unsigned int)((BitsInUnignedInt  * 3) / 4);
    unsigned int OneEighth        = (unsigned int)(BitsInUnignedInt / 8);
    unsigned int HighBits         = (unsigned int)(0xFFFFFFFF) << (BitsInUnignedInt - OneEighth);
    unsigned int hash             = 0;
    unsigned int test             = 0;

    while (*str) {
        hash = (hash << OneEighth) + (*str++);
        if ((test = hash & HighBits) != 0) {
            hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

    return (hash & 0x7FFFFFFF);
}

// ELF Hash Function
unsigned int ELFHash(char *str)
{
    unsigned int hash = 0;
    unsigned int x    = 0;

    while (*str) {
        hash = (hash << 4) + (*str++);
        if ((x = hash & 0xF0000000L) != 0) {
            hash ^= (x >> 24);
            hash &= ~x;
        }
    }

    return (hash & 0x7FFFFFFF);
}

// BKDR Hash Function
unsigned int BKDRHash(char *str)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;

    while (*str) {
        hash = hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

// DJB Hash Function
unsigned int DJBHash(char *str)
{
    unsigned int hash = 5381;

    while (*str) {
        hash += (hash << 5) + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

// AP Hash Function
unsigned int APHash(char *str)
{
    unsigned int hash = 0;
    int i;

    for (i=0; *str; i++) {
        if ((i & 1) == 0) {
            hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
        } else {
            hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
        }
    }

    return (hash & 0x7FFFFFFF);
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

void freeHashTable(hashTable *ht)
{
    for(int i=0; i<HASHTABLE_SIZE; i++) {
        entry *e = ht->list[i];
        if(!e) {
            free(e);
            continue;
        }
        while (e->pNext) {
            entry *next = e->pNext;
            e->pNext = next->pNext;
            free(next);
        }
        free(e);
    }
    free(ht->list);
    free(ht);
}

entry *findName(char lastName[], hashTable *ht, hashAlgo *algo)
{
    entry *e;
    unsigned int index = (algo->hashfuction(lastName)%HASHTABLE_SIZE);
    for(e = ht->list[index]; e; e = e->pNext) {
        if(!strcasecmp(lastName, e->lastName))
            return e;
    }
    return NULL;
}

void append(char lastName[], hashTable *hashTable, hashAlgo *algo)
{
    /* allocate memory for the new entry and put lastName */
    unsigned int index = (algo->hashfuction(lastName) % HASHTABLE_SIZE );
    entry *newEntry;
    newEntry = (entry *) malloc(sizeof(entry));
    newEntry->pNext = hashTable->list[index];
    strcpy(newEntry->lastName, lastName);
    hashTable->list[index] = newEntry;
}

hashAlgo SDBMHashProvider = {
    .hashfuction = SDBMHash,
    .init =initHashTable,
    .free = freeHashTable,
};


hashAlgo RSHashProvider = {
    .hashfuction = RSHash,
    .init =initHashTable,
    .free = freeHashTable,
};

hashAlgo JSHashProvider = {
    .hashfuction = JSHash,
    .init =initHashTable,
    .free = freeHashTable,
};

hashAlgo PJWHashProvider = {
    .hashfuction = PJWHash,
    .init =initHashTable,
    .free = freeHashTable,
};

hashAlgo ELFHashProvider = {
    .hashfuction = ELFHash,
    .init =initHashTable,
    .free = freeHashTable,
};
hashAlgo BKDRHashProvider = {
    .hashfuction = BKDRHash,
    .init =initHashTable,
    .free = freeHashTable,
};
hashAlgo DJBHashProvider = {
    .hashfuction = DJBHash,
    .init =initHashTable,
    .free = freeHashTable,
};
hashAlgo APHashProvider = {
    .hashfuction = APHash,
    .init =initHashTable,
    .free = freeHashTable,
};
