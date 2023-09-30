/******************************************************
 * Copyright Grégory Mounié 2018                      *
 * This code is distributed under the GLPv3+ licence. *
 * Ce code est distribué sous la licence GPLv3+.      *
 ******************************************************/

#include <stdint.h>
#include <assert.h>
#include "mem.h"
#include "mem_internals.h"

unsigned int puiss2(unsigned long size) {
    unsigned int p=0;
    size = size -1; // allocation start in 0
    while(size) {  // get the largest bit
	p++;
	size >>= 1;
    }
    if (size > (1 << p))
	p++;
    return p;
}


void *
emalloc_medium(unsigned long size)
{
    assert(size < LARGEALLOC);
    assert(size > SMALLALLOC);
    int index = puiss2(size + 32);
    void **ptr;
    void *left_buddy;
    void *right_buddy;
    if (arena.TZL[index] == NULL){
        int sup = index;
        while(arena.TZL[sup] == NULL){
            sup += 1;
            if (sup == FIRST_ALLOC_MEDIUM_EXPOSANT + arena.medium_next_exponant){
                mem_realloc_medium();
                ptr = arena.TZL[sup];
                *ptr = NULL;
            }
        }
        ptr = arena.TZL[sup];
        arena.TZL[sup] = NULL;
        while (sup > index){
            left_buddy = ptr;
            right_buddy = (void *)((unsigned long)left_buddy ^ (1 << (sup - 1)));
            //arena.TZL[sup] = *ptr;
            *ptr = NULL;
            ptr = right_buddy;
            sup -= 1;
            arena.TZL[sup] = left_buddy;
        }
        return mark_memarea_and_get_user_ptr(ptr, size+32, MEDIUM_KIND);
        
    }
    void *chunk = arena.TZL[index];
    ptr = chunk;
    arena.TZL[index] = *ptr;
    return mark_memarea_and_get_user_ptr(chunk, size+32, MEDIUM_KIND);
}


void helper(void* ptr, int index){
    void *current = arena.TZL[index];
    void *buddy = (void *)((long unsigned)ptr ^ (1 << index));
    void *previous = NULL;
    void **next = current;
    while (current){
        if (current == buddy){
            if (previous){
                next = previous;
                *next = *(void **)buddy;
            }
            else{
                next = buddy;
                arena.TZL[index] = *next;
            }
            if (ptr < buddy){
                helper(ptr, index + 1);
            }
            else{
                helper(buddy, index + 1);
            }
            return;
        }
        next = current;
        previous = current;
        current = *next;
    }
    next = ptr;
    *next = arena.TZL[index];
    arena.TZL[index] = ptr;
}

void efree_medium(Alloc a)
{
    unsigned int index = puiss2(a.size);
    helper(a.ptr, index);
}



