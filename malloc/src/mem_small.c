/******************************************************
 * Copyright Grégory Mounié 2018                      *
 * This code is distributed under the GLPv3+ licence. *
 * Ce code est distribué sous la licence GPLv3+.      *
 ******************************************************/

#include <assert.h>
#include "mem.h"
#include "mem_internals.h"

void *
emalloc_small(unsigned long size)
{
    if (arena.chunkpool == NULL){
        int nb_chunk = (mem_realloc_small()/CHUNKSIZE);
        void **ptr = arena.chunkpool;
        for (int i = 0; i< nb_chunk - 1 ; i+=1){
            *ptr = ptr + CHUNKSIZE/8;
            ptr = ptr + CHUNKSIZE/8;
        }
        *ptr = NULL;
    }
    void **chunkpool = arena.chunkpool;
    void *next_chunk = *chunkpool;
    void *output = mark_memarea_and_get_user_ptr(arena.chunkpool, CHUNKSIZE, SMALL_KIND);
    arena.chunkpool = next_chunk;
    return output;
}

void efree_small(Alloc a) {
    void **ptr = a.ptr;
    *ptr = arena.chunkpool;
    arena.chunkpool = a.ptr;
}



