#ifndef LIFE3D_GRID_H
#define LIFE3D_GRID_H

#include "hashtable.h"
#include "cell.h"

class Grid {
public:
    const unsigned size;
    const unsigned coord_max;
    hashtable_t *table[2] = {NULL, NULL};
    size_t state = 0;
    size_t n_cells[2];

    Grid(unsigned size, hashtable_t *ht, unsigned int num_cells) : size(size), coord_max(size - 1) {
        n_cells[state] = num_cells;
        table[state] = ht;
    };

    void getNeighbors(cell_t c, cell_t *neighbors);

    bool nextState(cell_t c);

    void evolve();
};

#endif //LIFE3D_GRID_H
