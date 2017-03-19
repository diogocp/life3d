#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "Cell.h"
#include "Grid.h"

#define buffer_size 300000

int read_file(const char *filename, cell_t *buffer, size_t max_n);

int read_grid_size(const char *filename);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Exactly two arguments expected (filename and number of generations))\n");
        return EXIT_FAILURE;
    }
    int n_generations = atoi(argv[2]);

    int grid_size = read_grid_size(argv[1]);
    if (grid_size < 1) {
        fprintf(stderr, "Invalid size: %u. Must be between 1 and 10000.\n", grid_size);
        return EXIT_FAILURE;
    }

    cell_t *buffer = (cell_t *) malloc(buffer_size * sizeof(cell_t));
    int ncells = read_file(argv[1], buffer, buffer_size);

    Grid grid = Grid(grid_size);
    grid.set(buffer, ncells);
    free(buffer);

    for(int i = 0; i < n_generations; i++) {
        grid.evolve();
    }
    grid.print();

    std::cerr << "Finished" << std::endl;
    return EXIT_SUCCESS;
}

int read_grid_size(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "File not found: %s\n", filename);
        return -2;
    }

    unsigned int size = 0;
    fscanf(file, "%u", &size);
    if (size < 1 || size > 10000) {
        fprintf(stderr, "Invalid size: %u. Must be between 1 and 10000.\n", size);
        fclose(file);
        return -3;
    }
    return size;
}

int read_file(const char *filename, cell_t *buffer, size_t max_n) {
    if (filename == NULL || buffer == NULL) {
        fprintf(stderr, "Filename and buffer must not be null\n");
        return -1;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "File not found: %s\n", filename);
        return -2;
    }

    unsigned int size = 0;
    fscanf(file, "%u", &size);
    if (size < 1 || size > 10000) {
        fprintf(stderr, "Invalid size: %u. Must be between 1 and 10000.\n", size);
        fclose(file);
        return -3;
    }
    fprintf(stderr, "Size: %u\n", size);

    unsigned n, x, y, z;
    for (n = 0; n < max_n; n++) {
        if (fscanf(file, "%u %u %u", &x, &y, &z) != EOF) {
            buffer[n] = CELL(x, y, z);
        } else {
            break;
        }
    }
    fclose(file);
    fprintf(stderr, "Read %u lines\n", n);

    return n;
}
