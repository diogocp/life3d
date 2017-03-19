#include <nmmintrin.h>
#include "HashTable.h"

uint32_t HashTable::hash(cell_t key) {
    return (uint32_t) _mm_crc32_u64(0, key);
}

size_t HashTable::findSlot(cell_t key) {
    size_t i = hash(key) % capacity;

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
