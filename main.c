#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <math.h>

#include IMPL

#ifdef OPT
#define OUT_FILE "opt.txt"
#else
#define OUT_FILE "orig.txt"
#endif

#ifdef HASH
#undef OUT_FILE
#define OUT_FILE "hash.txt"
#endif

#define DICT_FILE "./dictionary/words.txt"

#define SIMPLE_TIMES 20

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

int main(int argc, char *argv[])
{
    FILE *fp;
    int i = 0;
    char line[MAX_LAST_NAME_SIZE];
    struct timespec start, end;
    double cpu_time1, cpu_time2;

    /* check file opening */
    fp = fopen(DICT_FILE, "r");
    if (fp == NULL) {
        printf("cannot open the file\n");
        return -1;
    }

#ifdef HASH
    /* init hash table */
    hashTable *ht = initHashTable();
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
    int num = 0;
    double data[SIMPLE_TIMES];
    double sum = 0.0, average = 0.0, sd = 0.0, low_end = 0.0, up_end = 0.0;

    for (int times = 0; times < SIMPLE_TIMES; times++) {
        clock_gettime(CLOCK_REALTIME, &start);
        while (fgets(line, sizeof(line), fp)) {
            while (line[i] != '\0')
                i++;
            line[i - 1] = '\0';
            i = 0;
#ifdef HASH
            append(line, ht);
#else
            e = append(line, e);
#endif
        }
        clock_gettime(CLOCK_REALTIME, &end);

        fseek(fp, 0, SEEK_SET);
        /* calculate confidence interval */
        data[times] = diff_in_second(start, end);
        average += data[times];
    }
    average = average/SIMPLE_TIMES;

    for (int i = 0; i< SIMPLE_TIMES; i++)
        sd += (data[i] - average) * (data[i] - average);
    sd = sqrt(sd/SIMPLE_TIMES);

    low_end = average - 1.96*sd;
    up_end = average + 1.96*sd;
    for (int i = 0; i < SIMPLE_TIMES; i++) {
        if (data[i] >= low_end && data[i] <= up_end) {
            sum += data[i];
            num++;
        }
    }
    cpu_time1 = (double) sum/num;

    /* close file as soon as possible */
    fclose(fp);

    /* the given last name to find */
    char input[MAX_LAST_NAME_SIZE] = "zyxel";

    e = pHead;
#ifdef HASH
    assert(findName(input, ht) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, ht)->lastName, "zyxel"));
#else
    assert(findName(input, e) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e)->lastName, "zyxel"));
#endif

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    /* compute the execution time */
    average = 0.0;
    sd = 0.0;
    sum = 0.0;
    num = 0;
    for (int times = 0; times < SIMPLE_TIMES; times++) {
        clock_gettime(CLOCK_REALTIME, &start);
#ifdef HASH
        findName(input, ht);
#else
        findName(input, e);
#endif
        clock_gettime(CLOCK_REALTIME, &end);
        /* calculate confidence interval */
        data[times] = diff_in_second(start, end);
        average += data[times];
    }
    average = average/SIMPLE_TIMES;

    for (int i = 0; i < SIMPLE_TIMES; i++)
        sd += (data[i] - average) * (data[i] - average);
    sd = sqrt(sd/SIMPLE_TIMES);

    low_end = average - 1.96*sd;
    up_end = average + 1.96*sd;

    for (int i = 0; i < SIMPLE_TIMES; i++) {
        if (data[i] >= low_end && data[i] <= up_end) {
            sum += data[i];
            num++;
        }
    }
    cpu_time2 = (double) sum/num;

    FILE *output = fopen(OUT_FILE, "a");
    fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
    fclose(output);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);

#ifdef HASH
    freeHashTable(ht);
#endif

    while(pHead->pNext) {
        entry *next = pHead->pNext;
        pHead->pNext = next->pNext;
        free(next);
    }
    free(pHead);

    return 0;
}
