#include "hashtable.h"

#include <stdlib.h>
#include <nmmintrin.h>


hashtable_t *HT_create(size_t capacity) {
    hashtable_t *ht = (hashtable_t *) malloc(sizeof(hashtable_t));
    ht->capacity = capacity;
    ht->table = (unsigned long long int *) calloc(capacity, sizeof(unsigned long long int));
    return ht;
}

void HT_free(hashtable_t *ht) {
    if (ht == NULL)
        return;
    free(ht->table);
    free(ht);
}

size_t HT_find_slot(const hashtable_t *ht, unsigned long long int key) {
    size_t i = _mm_crc32_u64(0, key) % ht->capacity;

    while (ht->table[i] != 0 && ht->table[i] != key) {
        if (++i == ht->capacity) {
            i = 0;
        }
    }
    return i;
}

int HT_contains(const hashtable_t *ht, unsigned long long int key) {
    return ht->table[HT_find_slot(ht, key)] == key ? 1 : 0;
}

void HT_set(hashtable_t *ht, unsigned long long int key) {
    ht->table[HT_find_slot(ht, key)] = key;
}

void HT_set_atomic(hashtable_t *ht, unsigned long long int key) {
    size_t slot;
    do {
        slot = HT_find_slot(ht, key);
    } while (!__sync_bool_compare_and_swap(&(ht->table[slot]), 0, key) && ht->table[slot] != key);
}

void HT_set_all(hashtable_t *ht, const unsigned long long int *keys, size_t num_keys) {
    for(size_t i = 0; i < num_keys; i++) {
        HT_set(ht, keys[i]);
    }
}
