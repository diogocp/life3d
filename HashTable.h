#ifndef LIFE3D_HASHTABLE_H
#define LIFE3D_HASHTABLE_H

#include <stdlib.h>
#include <stdint.h>

#include "Cell.h"

class HashTable {
public:
    cell_t* table;
    size_t capacity;

    HashTable(size_t capacity) {
        this->capacity = capacity;
        table = (cell_t*)calloc(capacity, sizeof(cell_t));
    };

    ~HashTable() {
        free(table);
    }

    uint32_t hash(cell_t key);
    size_t findSlot(cell_t key);
    bool lookup(cell_t key);
    void set(cell_t key);
};


#endif //LIFE3D_HASHTABLE_H
