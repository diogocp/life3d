#include "Grid.h"


bool Grid::isAlive(cell_t cell) {
    //TODO hash table lookup
    return false;
}

bool Grid::nextState(cell_t c) {
    cell_t neighbors[6];
    Cell::getNeighbors(c, neighbors, size);

    int live_count = 0;
    for (int i = 0; i < 6; i++) {
        live_count += isAlive(neighbors[i]);
        if (live_count > 4)
            break;
        if (i == 4 && live_count == 0)
            break;
    }

    if (live_count < 2 || live_count > 4) {
        return 0;
    }
    return 1;
}
