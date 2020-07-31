#ifndef _ENCODING_TYPE_H_
#define _ENCODING_TYPE_H_
#include "formats.h"

void write_runs(rgb *pixels, int w, int h, int grayscale, FILE *file, char *table_size, rgb *table);
void raw_data(rgb *pixels, int w, int h, int grayscale, FILE *file, char *table_size, rgb *table);
void print_runs(int grayscale);
int *read_indices(int w, int h);
int *raw_indices(int w, int h);
void print_raw_data(int w, int h, int grayscale);

#endif /* _ENCODING_TYPE_H_ */

