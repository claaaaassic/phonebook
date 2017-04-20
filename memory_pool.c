#include <stdlib.h>

#include "memory_pool.h"

Memory_pool *mp_create(size_t size, size_t slots)
{
    Memory_pool *mp = (Memory_pool *) malloc(sizeof(Memory_pool));

    mp->head =(void *) malloc(size * slots);
    if(mp->head == NULL)
        return NULL;

    mp->tail = mp->head + size * slots;
    return mp;
}

void *mp_get(Memory_pool *mp, size_t size)
{
    if (mp->tail - mp->head < size)
        return NULL;

    void *tmp = mp->head;
    mp->head = mp->head + size;

    return tmp;
}

void mp_destory(Memory_pool *mp)
{
    free(mp);
}
