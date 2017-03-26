#include "hashtable.h"
#include "cell.h"

#include <omp.h>


void life3d_run(unsigned int size, hashtable_t *state, unsigned int num_cells, unsigned long generations) {
    hashtable_t *next_state;

    for (unsigned int g = 0; g < generations; g++) {
        next_state = HT_create(num_cells * 6);
        num_cells = 0;

        #pragma omp parallel for reduction(+:num_cells)
        for (unsigned int i = 0; i < state->capacity; i++) {
            cell_t c;
            cell_t neighbors[6];

            c = state->table[i];
            if (c == 0) continue;

            cell_get_neighbors(c, neighbors, size);

            if (cell_next_state(c, neighbors, state)) {
                HT_set_atomic(next_state, c);
                num_cells++;
            }

            cell_t buf[6];
            for (size_t j = 0; j < 6; j++) {
                c = neighbors[j];
                cell_get_neighbors(c, buf, size);

                if (!(HT_contains(state, c)) && !(HT_contains(next_state, c)) && cell_next_state(c, buf, state)) {
                    HT_set_atomic(next_state, c);
                    num_cells++;
                }
            }
        }

        HT_free(state);
        state = next_state;
    }
}
