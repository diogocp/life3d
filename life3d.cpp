#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "io.h"


static unsigned int next_generation(const hashtable_t *now, hashtable_t *next, unsigned int size);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s FILENAME GENERATIONS\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[1]);
        return EXIT_FAILURE;
    }

    unsigned long generations = strtoul(argv[2], NULL, 10);
    if (generations == 0 || generations > LONG_MAX) {
        fprintf(stderr, "%s: %s: Invalid number of generations\n", argv[0], argv[2]);
        return EXIT_FAILURE;
    }

    unsigned int size = read_size(file);
    if (size == 0 || size > 10000) {
        fprintf(stderr, "%s: %u: Invalid size\n", argv[0], size);
        return EXIT_FAILURE;
    }

    hashtable_t *initial_config;
    unsigned int num_cells = read_file(file, size, &initial_config);
    fclose(file);

    if (initial_config == NULL) {
        fprintf(stderr, "%s: Failed to read initial configuration from file\n", argv[0]);
    }

    fprintf(stderr, "Size: %u\nLines: %u\nGenerations: %lu\n", size, num_cells, generations);

    hashtable_t *now = initial_config;
    hashtable_t *next;

    for (unsigned int i = 0; i < generations; i++) {
        next = HT_create(num_cells * 2);
        num_cells = next_generation(now, next, size);
        HT_free(now);
        now = next;
    }
    print_cells(now);

    return EXIT_SUCCESS;
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
