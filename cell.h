#ifndef LIFE3D_CELL_H
#define LIFE3D_CELL_H

typedef unsigned long long int cell_t;

#define CELL(x, y, z) ((cell_t) (x & 0xffff) << 48 | (cell_t) (y & 0xffff) << 32 | (cell_t) (z & 0xffff) << 16 | 0xce11)
#define CELL_X(c) ((unsigned int)(c >> 48 & 0xffff))
#define CELL_Y(c) ((unsigned int)(c >> 32 & 0xffff))
#define CELL_Z(c) ((unsigned int)(c >> 16 & 0xffff))

#endif //LIFE3D_CELL_H
