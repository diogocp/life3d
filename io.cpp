#include <stdio.h>
#include <stdlib.h>

#include "io.h"

unsigned int estimate_lines(FILE *file, unsigned int size);

unsigned long long int ipow(unsigned int base, unsigned int exp);

unsigned long int get_file_size(FILE *file);

unsigned int read_size(FILE *file) {
    unsigned int size = 0;
    fscanf(file, "%u", &size);
    return size;
}

unsigned int read_file(FILE *file, unsigned int size, cell_t **cells) {
    unsigned int buffer_sz = estimate_lines(file, size);
    cell_t *buf = (cell_t *) malloc(buffer_sz * sizeof(cell_t));

    unsigned int x, y, z, n = 0;
    while (fscanf(file, "%u %u %u", &x, &y, &z) != EOF) {
        buf[n++] = CELL(x, y, z);
    }

    *cells = buf;
    return n;
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


#if defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))

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