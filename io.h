#ifndef LIFE3D_IO_H
#define LIFE3D_IO_H

#include "hashtable.h"
#include "cell.h"

#include <stdio.h>


unsigned int read_size(FILE *file);

unsigned int read_file(FILE *file, unsigned int size, cell_t **cells);

void print_cells(hashtable_t *ht);

#endif //LIFE3D_IO_H
