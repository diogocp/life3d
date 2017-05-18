#ifndef LIFE3D_HASHTABLE_H
#define LIFE3D_HASHTABLE_H

#include <stddef.h>


typedef struct {
    unsigned long long int *table;
    size_t capacity;
} hashtable_t;

hashtable_t *HT_create(size_t capacity);

void HT_free(hashtable_t *t);

int HT_contains(const hashtable_t *ht, unsigned long long int key);

void HT_set(hashtable_t *ht, unsigned long long int key);

void HT_set_atomic(hashtable_t *ht, unsigned long long int key);

void HT_set_all(hashtable_t *ht, const unsigned long long int *keys, size_t num_keys);

#endif //LIFE3D_HASHTABLE_H
