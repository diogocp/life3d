#include "cell.h"
#include "hashtable.h"


void cell_get_neighbors(cell_t c, cell_t *neighbors, unsigned int size) {
    unsigned int x = CELL_X(c);
    unsigned int y = CELL_Y(c);
    unsigned int z = CELL_Z(c);

    neighbors[0] = CELL((x - 1 + size) % size, y, z);
    neighbors[1] = CELL((x + 1 + size) % size, y, z);
    neighbors[2] = CELL(x, (y - 1 + size) % size, z);
    neighbors[3] = CELL(x, (y + 1 + size) % size, z);
    neighbors[4] = CELL(x, y, (z - 1 + size) % size);
    neighbors[5] = CELL(x, y, (z + 1 + size) % size);
}

int cell_next_state(cell_t c, const cell_t *neighbors, const hashtable_t *table) {
    int live_count = 0;
    for (int i = 0; i < 6; i++) {
        live_count += HT_contains(table, neighbors[i]);
        if (live_count > 4)
            break;
        if (i == 4 && live_count == 0)
            break;
    }

    if (live_count < 2 || live_count > 4) {
        return 0;
    }
    if (!HT_contains(table, c) && live_count == 4) {
        return 0;
    }
    return 1;
}
