#ifndef LIFE3D_IO_H
#define LIFE3D_IO_H

#include "hashtable.h"

#include <stdio.h>


unsigned int read_size(FILE *file);

unsigned int read_file(FILE *file, unsigned int size, hashtable_t **cells_ht);

void print_cells(hashtable_t *ht);

#endif //LIFE3D_IO_H
