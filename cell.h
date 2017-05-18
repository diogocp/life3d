#ifndef LIFE3D_CELL_H
#define LIFE3D_CELL_H

#include "hashtable.h"


typedef unsigned long long int cell_t;

#define CELL(x, y, z) ((cell_t)((x) & 0xffff) << 48 | (cell_t)((y) & 0xffff) << 32 | (cell_t)((z) & 0xffff) << 16 | 0xce11)
#define CELL_X(c) ((unsigned int)((c) >> 48 & 0xffff))
#define CELL_Y(c) ((unsigned int)((c) >> 32 & 0xffff))
#define CELL_Z(c) ((unsigned int)((c) >> 16 & 0xffff))

void cell_get_neighbors(cell_t c, cell_t *neighbors, unsigned int size);

int cell_next_state(cell_t c, const cell_t *neighbors, const hashtable_t *table);

int compare_cells(const void *a, const void *b);

cell_t cell_block_low(const int *coords, const int *dims, unsigned int size);

cell_t cell_block_high(const int *coords, const int *dims, unsigned int size);

int in_region(cell_t c, cell_t lower_bound, cell_t upper_bound, int size, int overlap);

#endif //LIFE3D_CELL_H
