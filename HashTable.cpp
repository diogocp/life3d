#include <stdlib.h>
#include <nmmintrin.h>

#include "HashTable.h"

HashTable::HashTable(size_t capacity) {
    this->capacity = capacity;
    table = (cell_t *) calloc(capacity, sizeof(cell_t));
}

HashTable::~HashTable() {
    free(table);
}

size_t HashTable::findSlot(cell_t key) {
    size_t i = (unsigned int) _mm_crc32_u64(0, key) % capacity;

    while (table[i] != 0 && table[i] != key) {
        if (++i == capacity) {
            i = 0;
        }
    }
    return i;
}

bool HashTable::contains(cell_t key) {
    return table[findSlot(key)] == key;
}

void HashTable::set(cell_t key) {
    table[findSlot(key)] = key;
}
