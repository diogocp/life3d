#include "Cell.h"

using namespace Cell;
using namespace std;

cell_t Cell::create(unsigned x, unsigned y, unsigned z) {
    return (cell_t) (x & 0xffff) << 16 | (cell_t) (y & 0xffff) << 32 | (cell_t) (z & 0xffff) << 48 | 1;
}

unsigned Cell::getX(cell_t c) {
    return (unsigned) (c >> 16 & 0xffff);
}

unsigned Cell::getY(cell_t c) {
    return (unsigned) (c >> 32 & 0xffff);
}

unsigned Cell::getZ(cell_t c) {
    return (unsigned) (c >> 48 & 0xffff);
}

void Cell::getNeighbors(cell_t c, cell_t *neighbors, unsigned coord_max) {
    unsigned x = getX(c);
    unsigned y = getY(c);
    unsigned z = getZ(c);

    if (x == 0) {
        neighbors[0] = create(coord_max, y, z);
    } else {
        neighbors[0] = create(x - 1, y, z);
    }
    if (x == coord_max) {
        neighbors[1] = create(0, y, z);
    } else {
        neighbors[1] = create(x + 1, y, z);
    }

    if (y == 0) {
        neighbors[2] = create(x, coord_max, z);
    } else {
        neighbors[2] = create(x, y - 1, z);
    }
    if (y == coord_max) {
        neighbors[3] = create(x, 0, z);
    } else {
        neighbors[3] = create(x, y + 1, z);
    }

    if (z == 0) {
        neighbors[4] = create(x, y, coord_max);
    } else {
        neighbors[4] = create(x, y, z - 1);
    }
    if (z == coord_max) {
        neighbors[5] = create(x, y, 0);
    } else {
        neighbors[5] = create(x, y, z + 1);
    }
}
