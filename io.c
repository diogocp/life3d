#include "io.h"
#include "hashtable.h"
#include "cell.h"

#include <stdlib.h>
#include <stdio.h>


unsigned int estimate_lines(FILE *file, unsigned int size);

unsigned long long int ipow(unsigned int base, unsigned int exp);

unsigned long int get_file_size(FILE *file);


unsigned int read_size(FILE *file) {
    unsigned int size = 0;
    fscanf(file, "%u", &size);
    return size;
}

unsigned int read_file(FILE *file, unsigned int size, hashtable_t **cells_ht) {
    hashtable_t *ht = HT_create(estimate_lines(file, size) * 2);

    unsigned int x, y, z, n = 0;
    while (fscanf(file, "%u %u %u", &x, &y, &z) != EOF) {
        HT_set(ht, CELL(x, y, z));
        n++;
    }

    *cells_ht = ht;
    return n;
}

void print_cells(hashtable_t *ht) {
    int compareCells(const void *a, const void *b);
    qsort(ht->table, ht->capacity, sizeof(cell_t), compareCells);

    cell_t c;
    for (unsigned int i = 0; i < ht->capacity; i++) {
        c = ht->table[i];
        if (c != 0) {
            fprintf(stdout, "%u %u %u\n", CELL_X(c), CELL_Y(c), CELL_Z(c));
        }
    }
}

/**
 * This function gives an upper bound on the number of lines in the file,
 * by assuming that each line is as small as possible.
 */
unsigned int estimate_lines(FILE *file, unsigned int size) {
    unsigned int lines;
    unsigned long file_size = get_file_size(file);

    if (size <= 10 || file_size <= 6000) {
        // This case is easy since all coordinates have
        // a single digit (or anyway we assume that).
        return (unsigned int) (file_size / 6 + 1);
    } else {
        lines = 1000;
        file_size -= 6000;
    }

    // Don't even try to read this part
    unsigned int factor = 10;
    unsigned int line_size = 6;
    unsigned long long int cases;

    for (unsigned int base = 100; base <= 10000; base *= 10) {
        unsigned int prev_factor = factor;
        factor = size < base ? size - base / 10 : base - base / 10;

        for (unsigned int i = 1; i <= 3; i++) {
            line_size++;
            cases = ipow(factor, i) * ipow(prev_factor, 3 - i);

            if (file_size < cases * line_size) {
                return (unsigned int) (lines + file_size / line_size + 1);
            } else {
                lines += cases;
                file_size -= cases * line_size;
            }
        }
    }

    return lines;
}

unsigned long long int ipow(unsigned int base, unsigned int exp) {
    unsigned long long int result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }
    return result;
}


#ifdef _POSIX_SOURCE

#include <sys/stat.h>

// On UNIX use fstat to avoid seeking
unsigned long int get_file_size(FILE *file) {
    struct stat buf;
    fstat(fileno(file), &buf);
    return buf.st_size < 0 ? 0 : (unsigned long int) buf.st_size;

}

#else

unsigned long int get_file_size(FILE *file) {
    long current_pos, file_size;

    current_pos = ftell(file);
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, current_pos, SEEK_SET);

    return file_size < 0 ? 0 : (unsigned long int) file_size;
}

#endif

int compareCells(const void *a, const void *b) {
    if (*(cell_t *) a < *(cell_t *) b)
        return -1;
    if (*(cell_t *) a > *(cell_t *) b)
        return 1;
    return 0;
}
