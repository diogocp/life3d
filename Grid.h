#ifndef LIFE3D_GRID_H
#define LIFE3D_GRID_H

#include "Cell.h"
#include "HashTable.h"

class Grid {
public:
    const unsigned size;
    HashTable *table[2] = {NULL, NULL};
    size_t state = 0;
    size_t n_cells[2] = {0, 0};

    Grid(unsigned size) : size(size) {};

    ~Grid() {
        delete table[0];
        delete table[1];
    }

    bool isAlive(cell_t cell);

    bool nextState(cell_t c);

    void set(cell_t *cells, size_t n_cells);

    void evolve();

    void print();
};

#endif //LIFE3D_GRID_H
