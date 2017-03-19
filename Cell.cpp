#include "Cell.h"

void Cell::getNeighbors(cell_t c, cell_t *neighbors, unsigned coord_max) {
    unsigned x = CELL_X(c);
    unsigned y = CELL_Y(c);
    unsigned z = CELL_Z(c);

    if (x == 0) {
        neighbors[0] = CELL(coord_max, y, z);
    } else {
        neighbors[0] = CELL(x - 1, y, z);
    }
    if (x == coord_max) {
        neighbors[1] = CELL(0, y, z);
    } else {
        neighbors[1] = CELL(x + 1, y, z);
    }

    if (y == 0) {
        neighbors[2] = CELL(x, coord_max, z);
    } else {
        neighbors[2] = CELL(x, y - 1, z);
    }
    if (y == coord_max) {
        neighbors[3] = CELL(x, 0, z);
    } else {
        neighbors[3] = CELL(x, y + 1, z);
    }

    if (z == 0) {
        neighbors[4] = CELL(x, y, coord_max);
    } else {
        neighbors[4] = CELL(x, y, z - 1);
    }
    if (z == coord_max) {
        neighbors[5] = CELL(x, y, 0);
    } else {
        neighbors[5] = CELL(x, y, z + 1);
    }
}
