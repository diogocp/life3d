#include <cstdio>
#include <cstdlib>

#include "Grid.h"

void Grid::getNeighbors(cell_t c, cell_t *neighbors) {
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

int Grid::isAlive(cell_t cell) {
    return HT_contains(table[state], cell);
}

bool Grid::nextState(cell_t c) {
    cell_t neighbors[6];
    getNeighbors(c, neighbors);

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
    if (!isAlive(c) && live_count == 4) {
        return 0;
    }
    return 1;
}

void Grid::set(cell_t *cells, size_t n_cells) {
    table[state] = HT_create(n_cells * 2);
    for (unsigned int i = 0; i < n_cells; i++) {
        HT_set(table[state], cells[i]);
    }
    this->n_cells[state] = n_cells;
}

void Grid::evolve() {
    HT_free(table[state ^ 1]);
    table[state ^ 1] = HT_create(n_cells[state] * 2);

    hashtable_t *now = table[state];
    hashtable_t *next = table[state ^ 1];

    unsigned int n_cells_next = 0;
    cell_t c;
    cell_t neighbors[6];
    for (unsigned int i = 0; i < now->capacity; i++) {
        c = now->table[i];
        if (c == 0) continue;

        if (nextState(c)) {
            HT_set(next, c);
            n_cells_next++;
        }

        getNeighbors(c, neighbors);

        for (size_t j = 0; j < 6; j++) {
            c = neighbors[j];
            if (!(HT_contains(now, c)) && !(HT_contains(next, c)) && nextState(c)) {
                HT_set(next, c);
                n_cells_next++;
            }
        }
    }

    state ^= 1;
    n_cells[state] = n_cells_next;
    return;
}

int compareCells(const void *a, const void *b) {
    if (*(cell_t *) a < *(cell_t *) b)
        return -1;
    if (*(cell_t *) a > *(cell_t *) b)
        return 1;
    return 0;
}

void Grid::print() {
    cell_t c;

    std::qsort(table[state]->table, table[state]->capacity, sizeof(cell_t), compareCells);

    for (unsigned int i = 0; i < table[state]->capacity; i++) {
        c = table[state]->table[i];
        if (c != 0) {
            std::fprintf(stdout, "%u %u %u\n", CELL_X(c), CELL_Y(c), CELL_Z(c));
        }
    }
}
