#ifndef _MEMORY_POOL_H
#define _MEMORY_POOL_G

#include <stdlib.h>

typedef struct memory_pool {
    void *head;
    void *tail;
} Memory_pool;

Memory_pool *mp_create(size_t size, size_t slots);
void *mp_get(Memory_pool *mp, size_t size);
void mp_destory(Memory_pool *mp);

#endif
