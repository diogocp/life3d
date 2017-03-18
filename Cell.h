#ifndef LIFE3D_CELL_H
#define LIFE3D_CELL_H

#include <cstdint>

typedef std::int64_t cell_t;

namespace Cell {
    cell_t create(unsigned x, unsigned y, unsigned z);

    unsigned getX(cell_t c);

    unsigned getY(cell_t c);

    unsigned getZ(cell_t c);

    void getNeighbors(cell_t c, cell_t *neighbors, unsigned coord_max);
};

#endif //LIFE3D_CELL_H
