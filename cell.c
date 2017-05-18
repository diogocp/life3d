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


int compare_cells(const void *a, const void *b) {
    if (*(cell_t *) a < *(cell_t *) b)
        return -1;

    if (*(cell_t *) a > *(cell_t *) b)
        return 1;

    return 0;
}

/* Lower bound of the block decomposition */
cell_t cell_block_low(const int *coords, const int *dims, unsigned int size) {
    return CELL(coords[0] * size / dims[0],
                coords[1] * size / dims[1],
                coords[2] * size / dims[2]);
}

cell_t cell_block_high(const int *coords, const int *dims, unsigned int size) {
    return CELL((coords[0] + 1) * size / dims[0] - 1,
                (coords[1] + 1) * size / dims[1] - 1,
                (coords[2] + 1) * size / dims[2] - 1);
}

/* Returns 1 if c is in the region bounded by lower_bound and upper_bound or a region of size overlap
 * above or below those bounds. Returns 0 otherwise.
 */
int in_region(cell_t c, cell_t lower_bound, cell_t upper_bound, int size, int overlap) {
    int cell_coords[] = {CELL_X(c), CELL_Y(c), CELL_Z(c)};

    int min_coords[] = {CELL_X(lower_bound), CELL_Y(lower_bound), CELL_Z(lower_bound)};
    int max_coords[] = {CELL_X(upper_bound), CELL_Y(upper_bound), CELL_Z(upper_bound)};

    for (int dim = 0; dim < 3; dim++) {
        if (min_coords[dim] - overlap >= 0 && max_coords[dim] + overlap <= size) {
            // No underflow or overflow; normal case when region is away from the edges
            if (min_coords[dim] - overlap > cell_coords[dim] || cell_coords[dim] > max_coords[dim] + overlap) {
                return 0;
            }
        }
        if (min_coords[dim] - overlap < 0 && max_coords[dim] + overlap > size) {
            // Both underflow and overflow: region is the entire dimension
            continue;
        }
        if (min_coords[dim] - overlap < 0) {
            // Underflow only
            if (!(min_coords[dim] - overlap + size <= cell_coords[dim] ||
                  cell_coords[dim] <= max_coords[dim] + overlap)) {
                return 0;
            }
        }
        if (max_coords[dim] + overlap > size) {
            // Overflow only
            if (!(min_coords[dim] - overlap <= cell_coords[dim] ||
                  cell_coords[dim] <= max_coords[dim] + overlap - size)) {
                return 0;
            }
        }
    }
    return 1;
}
