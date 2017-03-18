#include "HashTable.h"
#include "MurmurHash3.h"

size_t HashTable::hash(cell_t key) {
    size_t h;
    MurmurHash3_x86_32(&key, sizeof(cell_t), 42, &h);
    return h;
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

bool HashTable::lookup(cell_t key) {
    return table[findSlot(key)] == key;
}

void HashTable::set(cell_t key) {
    table[findSlot(key)] = key;
}
