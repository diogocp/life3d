#ifndef LIFE3D_HASHTABLE_H
#define LIFE3D_HASHTABLE_H

#include "cell.h"

class HashTable {
public:
    cell_t *table;
    size_t capacity;

    HashTable(size_t capacity);
    ~HashTable();

    size_t findSlot(cell_t key);
    bool contains(cell_t key);
    void set(cell_t key);
};

#endif //LIFE3D_HASHTABLE_H
