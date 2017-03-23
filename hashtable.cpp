#include "hashtable.h"

#include <stdlib.h>
#include <nmmintrin.h>


hashtable_t *HT_create(size_t capacity) {
    hashtable_t *ht = (hashtable_t *) malloc(sizeof(hashtable_t));
    ht->capacity = capacity;
    ht->table = (cell_t *) calloc(capacity, sizeof(cell_t));
    return ht;
}

void HT_free(hashtable_t *ht) {
    if (ht == NULL)
        return;
    free(ht->table);
    free(ht);
}

size_t HT_find_slot(const hashtable_t *ht, cell_t key) {
    size_t i = (unsigned int) _mm_crc32_u64(0, key) % ht->capacity;

    while (ht->table[i] != 0 && ht->table[i] != key) {
        if (++i == ht->capacity) {
            i = 0;
        }
    }
    return i;
}

int HT_contains(const hashtable_t *ht, cell_t key) {
    return ht->table[HT_find_slot(ht, key)] == key ? 1 : 0;
}

void HT_set(hashtable_t *ht, cell_t key) {
    ht->table[HT_find_slot(ht, key)] = key;
}
