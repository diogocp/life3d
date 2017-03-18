#ifndef LIFE3D_GRID_H
#define LIFE3D_GRID_H

#include "Cell.h"

class Grid {
    unsigned size;
    // hash table for the state
    // hash table for the next state

    bool isAlive(cell_t cell);

    bool nextState(cell_t c);
};

#endif //LIFE3D_GRID_H
