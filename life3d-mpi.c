#include "hashtable.h"
#include "cell.h"
#include "life3d.h"

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

const int OVERLAP = 5;

static unsigned int next_generation(const hashtable_t *now, hashtable_t *next, unsigned int size);

static unsigned int get_cells_in_region(const hashtable_t *ht, cell_t lower_bound, cell_t upper_bound,
                                        unsigned int size, int overlap, cell_t *out);

void life3d_run(unsigned int size, hashtable_t *state, unsigned int num_cells, unsigned long generations) {

    // MPI grid initialization
    int grid_rank, num_procs;
    int dims[3] = {0, 0, 0};
    int coords[3] = {0, 0, 0};
    const int periodic[] = {1, 1, 1};
    MPI_Comm grid_comm;

    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Dims_create(num_procs, 3, dims);
    MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periodic, 1, &grid_comm);
    MPI_Comm_rank(grid_comm, &grid_rank);
    MPI_Cart_coords(grid_comm, grid_rank, 3, coords);

    // Get the bounds of this process
    cell_t lower_bound = cell_block_low(coords, dims, size);
    cell_t upper_bound = cell_block_high(coords, dims, size);

    // Make a list of cells needed by this process
    cell_t *my_cells = (cell_t *) calloc(num_cells, sizeof(cell_t));
    num_cells = get_cells_in_region(state, lower_bound, upper_bound, size, OVERLAP, my_cells);

    // Create a new hash table with only the cells of this process
    HT_free(state);
    state = HT_create(num_cells * 3);
    HT_set_all(state, my_cells, num_cells);
    free(my_cells);
    my_cells = NULL;

    // DEBUG
    fprintf(stderr, "%d/%d: %u cells / dims (%d,%d,%d) / coords (%d,%d,%d) / bounds (%d,%d,%d) -> (%d,%d,%d) \n",
            grid_rank, num_procs, num_cells,
            dims[0], dims[1], dims[2],
            coords[0], coords[1], coords[2],
            CELL_X(lower_bound), CELL_Y(lower_bound), CELL_Z(lower_bound),
            CELL_X(upper_bound), CELL_Y(upper_bound), CELL_Z(upper_bound));

    // Run the generations loop
    hashtable_t *next_state;
    for (unsigned int i = 0; i < generations; i++) {
        next_state = HT_create(num_cells * 6);
        num_cells = next_generation(state, next_state, size);
        HT_free(state);
        state = next_state;
    }

    // Gather the number of cells of each process
    my_cells = (cell_t *) calloc(num_cells, sizeof(cell_t));
    num_cells = get_cells_in_region(state, lower_bound, upper_bound, size, 0, my_cells);
    int num_cells_by_process[num_procs];
    MPI_Gather(&num_cells, 1, MPI_UNSIGNED, num_cells_by_process, 1, MPI_INT, 0, grid_comm);

    // Calculate the displacements for gathering the cells
    int displs[num_procs];
    displs[0] = 0;
    for (int p = 1; p < num_procs; p++) {
        displs[p] = displs[p - 1] + num_cells_by_process[p - 1];
    }
    unsigned int total_cells = (unsigned int) displs[num_procs - 1] + num_cells_by_process[num_procs - 1];

    if (grid_rank == 0) {
        for (int p = 0; p < num_procs; p++) {
            fprintf(stderr, "Process %d sending %d cells\n", p, num_cells_by_process[p]);
        }
        fprintf(stderr, "Total cells: %d\n", total_cells);
    }

    // Create a buffer to hold all the cells on the root process
    cell_t *all_cells = NULL;
    if (grid_rank == 0) {
        all_cells = (cell_t *) calloc(total_cells, sizeof(cell_t));
    }

    // Gather the cells
    MPI_Gatherv(my_cells, num_cells, MPI_UNSIGNED_LONG_LONG, all_cells, num_cells_by_process, displs,
                MPI_UNSIGNED_LONG_LONG, 0, grid_comm);

    // All non-root processes are done
    if (grid_rank != 0)
        return;

    // Sort the list of all cells
    qsort(all_cells, total_cells, sizeof(cell_t), compare_cells);

    // Print the cells
    for (unsigned int i = 0; i < total_cells; i++) {
        fprintf(stdout, "%u %u %u\n", CELL_X(all_cells[i]), CELL_Y(all_cells[i]), CELL_Z(all_cells[i]));
    }
}

static unsigned int get_cells_in_region(const hashtable_t *ht, cell_t lower_bound, cell_t upper_bound,
                                        unsigned int size, int overlap, cell_t *out) {
    unsigned int num_cells = 0;

    cell_t c;
    for (unsigned int i = 0; i < ht->capacity; i++) {
        c = ht->table[i];
        if (c == 0)
            continue;

        if (in_region(c, lower_bound, upper_bound, size, overlap))
            out[num_cells++] = c;
    }

    return num_cells;
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
