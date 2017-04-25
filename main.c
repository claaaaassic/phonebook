#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
//#include "phonebook_hash.h"

#include IMPL

#ifdef OPT
#define OUT_FILE "opt.txt"
#elif defined(HASH)
#define OUT_FILE "hash.txt"
#define OUT_FILE_NAME(ALGONAME,INDX) ALGONAME[INDX]
#else
#define OUT_FILE "orig.txt"
#endif


#define DICT_FILE "./dictionary/words.txt"

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}


#ifdef HASH

hashAlgo * hash_function_providers[] = {
    &SDBMHashProvider,
    &RSHashProvider,
    &JSHashProvider,
    &PJWHashProvider,
    &ELFHashProvider,
    &BKDRHashProvider,
    &DJBHashProvider,
    &APHashProvider
};


char * algo_out_path[] = {
    "SDBM.txt",
    "RS.txt",
    "JS.txt",
    "PJW.txt",
    "ELF.txt",
    "BKDR.txt",
    "DJB.txt",
    "AP.txt"
};




#endif

int main(int argc, char *argv[])
{

    FILE *fp;
    int i = 0;
    int index=5;
    char line[MAX_LAST_NAME_SIZE];
    struct timespec start, end;
    double cpu_time1, cpu_time2;


    /* check file opening */
    fp = fopen(DICT_FILE, "r");
    if (fp == NULL) {
        printf("cannot open the file\n");
        return -1;
    }



#ifdef HASHALGO

    for (index = 0; index < 8; index++) {

        fp = fopen(DICT_FILE, "r");
        if (fp == NULL) {
            printf("cannot open the file\n");
            return -1;
        }
        unsigned int hash_table_count [HASHTABLE_SIZE] = {};
        char line[MAX_LAST_NAME_SIZE]= {};
        hashAlgo *algo = hash_function_providers[index];
        while (fgets(line, sizeof(line), fp)) {
            while (line[i] != '\0')
                i++;
            line[i - 1] = '\0';
            i = 0;
            hash_table_count[algo->hashfuction(line) % HASHTABLE_SIZE]++;
        }
        FILE *output1 = fopen( OUT_FILE_NAME(algo_out_path,index), "a");
        for(i=0 ; i<HASHTABLE_SIZE ; i++) {
            fprintf(output1, "%d %d\n", i, hash_table_count[i]);
        }
        fclose(output1);
        i=0;
        fclose(fp);
    }
    return ;
#endif


#ifdef  HASH
    /* init hash table */
    hashAlgo *algo = hash_function_providers[5];
    hashTable *ht = algo->init();
#endif

    /* build the entry */
    entry *pHead, *e;
    pHead = (entry *) malloc(sizeof(entry));
    printf("size of entry : %lu bytes\n", sizeof(entry));
    e = pHead;
    e->pNext = NULL;

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    clock_gettime(CLOCK_REALTIME, &start);


    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
#ifdef HASH
        append(line, ht, algo);

#else
        e = append(line, e);
#endif
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* close file as soon as possible */
    fclose(fp);

    /* the given last name to find */
    char input[MAX_LAST_NAME_SIZE] = "zyxel";

    e = pHead;
#ifdef HASH
    assert(findName(input, ht, algo) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, ht, algo)->lastName, "zyxel"));
#else
    assert(findName(input, e) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e)->lastName, "zyxel"));
#endif

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    /* compute the execution time */
    clock_gettime(CLOCK_REALTIME, &start);
#ifdef HASH
    findName(input, ht, algo);

#else
    findName(input, e);
#endif
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    FILE *output = fopen(OUT_FILE, "a");
    fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
    fclose(output);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);

#ifdef HASH
    algo->free(ht);
#endif

    while(pHead->pNext) {
        entry *next = pHead->pNext;
        pHead->pNext = next->pNext;
        free(next);
    }
    free(pHead);



    return 0;
}
