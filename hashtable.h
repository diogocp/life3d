#ifndef LIFE3D_HASHTABLE_H
#define LIFE3D_HASHTABLE_H

#include "cell.h"

#include <stddef.h>


typedef struct {
    cell_t *table;
    size_t capacity;
} hashtable_t;

hashtable_t *HT_create(size_t capacity);

void HT_free(hashtable_t *t);

int HT_contains(const hashtable_t *ht, cell_t key);

void HT_set(hashtable_t *ht, cell_t key);

#endif //LIFE3D_HASHTABLE_H
