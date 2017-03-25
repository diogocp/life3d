#include "hashtable.h"
#include "cell.h"


static unsigned int next_generation(const hashtable_t *now, hashtable_t *next, unsigned int size);

void life3d_run(unsigned int size, hashtable_t *state, unsigned int num_cells, unsigned long generations) {
    hashtable_t *next_state;

    for (unsigned int i = 0; i < generations; i++) {
        next_state = HT_create(num_cells * 2);
        num_cells = next_generation(state, next_state, size);
        HT_free(state);
        state = next_state;
    }
}

static unsigned int next_generation(const hashtable_t *now, hashtable_t *next, unsigned int size) {
    unsigned int ncells_next = 0;

    cell_t c;
    cell_t neighbors[6];

    for (unsigned int i = 0; i < now->capacity; i++) {
        c = now->table[i];
        if (c == 0) continue;

        cell_get_neighbors(c, neighbors, size);

        if (cell_next_state(c, neighbors, now)) {
            HT_set(next, c);
            ncells_next++;
        }

        cell_t buf[6];
        for (size_t j = 0; j < 6; j++) {
            c = neighbors[j];
            cell_get_neighbors(c, buf, size);

            if (!(HT_contains(now, c)) && !(HT_contains(next, c)) && cell_next_state(c, buf, now)) {
                HT_set(next, c);
                ncells_next++;
            }
        }
    }

    return ncells_next;
}
