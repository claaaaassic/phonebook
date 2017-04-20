#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>

#include IMPL
#include "file_align.h"
#include "memory_pool.h"

#define OUT_FILE "mmap.txt"

#define DICT_FILE "./dictionary/words.txt"
#define FILE_OUT "input.txt"

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
    int fd, data_count;
    off_t size;
    char *map;
    struct timespec start, end;
    double cpu_time1, cpu_time2;

    /* build the entry */
    entry *pHead, *e;
    pHead = (entry *) malloc(sizeof(entry));
    printf("size of entry : %lu bytes\n", sizeof(entry));
    e = pHead;
    e->pNext = NULL;

    file_align(DICT_FILE, FILE_OUT, MAX_LAST_NAME_SIZE);

    fd = open(FILE_OUT, O_RDONLY | O_NONBLOCK);
    if (!fd) {
        printf("cannot open the file\n");
        return -1;
    }
    size = fsize(FILE_OUT);
    data_count = size / (sizeof(char) * MAX_LAST_NAME_SIZE);

    map = mmap(NULL, size, PROT_READ, MAP_SHARED | MAP_NONBLOCK, fd, 0);

    /* build a memory pool*/
    Memory_pool *mp = mp_create(sizeof(entry), data_count);
    if (!mp)
        printf("memory pool create error!\n");

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    clock_gettime(CLOCK_REALTIME, &start);

    for (int i = 0; i < data_count; i++) {
        entry *next = (entry *) mp_get(mp, sizeof(entry));

        if (!next)
            e = append(map + i * MAX_LAST_NAME_SIZE, e);
        else
            e = append_mp(map + i * MAX_LAST_NAME_SIZE, e, next);
    }

    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* the given last name to find */
    char input[MAX_LAST_NAME_SIZE] = "zyxel";

    e = pHead;

    assert(findName(input, e) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e)->lastName, "zyxel"));

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    /* compute the execution time */
    clock_gettime(CLOCK_REALTIME, &start);

    findName(input, e);

    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    FILE *output = fopen(OUT_FILE, "a");
    fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
    fclose(output);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);

    mp_destory(mp);

    return 0;
}
