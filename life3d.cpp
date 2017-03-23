#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "io.h"
#include "Grid.h"

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

    Grid grid = Grid(size, initial_config, num_cells);

    for (unsigned int i = 0; i < generations; i++) {
        grid.evolve();
    }
    print_cells(grid.table[grid.state]);

    return EXIT_SUCCESS;
}
