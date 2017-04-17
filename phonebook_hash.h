#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#define MAX_LAST_NAME_SIZE 16
#define HASHTABLE_SIZE 3000

#define HASH 1
typedef struct __PHONE_BOOK_DETAIL {
    char firstName[16];
    char email[16];
    char phone[10];
    char cell[10];
    char addr1[16];
    char addr2[16];
    char city[16];
    char state[2];
    char zip[5];
} detail;

typedef struct __PHONE_BOOK_ENTRY {
    char lastName[MAX_LAST_NAME_SIZE];
    detail *detail;
    struct __PHONE_BOOK_ENTRY *pNext;
} entry;

typedef struct __PHONE_BOOK_HASHTABLE {
    entry **list;
} hashTable;

typedef struct __HASH_ALGO {
    unsigned int (*hashfuction)(char *str);
    hashTable* (*init)();
    void (*free)(hashTable *ht );
} hashAlgo;


entry* findName(char lastName[], hashTable *ht, hashAlgo *algo);
void append(char lastName[], hashTable *ht, hashAlgo *algo);

extern hashAlgo SDBMHashProvider;
extern hashAlgo RSHashProvider;
extern hashAlgo JSHashProvider;
extern hashAlgo PJWHashProvider;
extern hashAlgo ELFHashProvider;
extern hashAlgo BKDRHashProvider;
extern hashAlgo DJBHashProvider;
extern hashAlgo APHashProvider;

#endif
