#ifndef LIFE3D_IO_H
#define LIFE3D_IO_H

#include "cell.h"

unsigned int read_size(FILE *file);

unsigned int read_file(FILE *file, unsigned int size, cell_t **cells);

#endif //LIFE3D_IO_H
