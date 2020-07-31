#ifndef _COLOR_TABLE_H_
#define _COLOR_TABLE_H_
#include "formats.h"

typedef 
struct cluster_mean_info{
  unsigned long r_squares_sum;
  unsigned long g_squares_sum;
  unsigned long b_squares_sum;
  int num_colors;
} cluster_mean_info;

rgb *color_table(rgb *pixels, int table_size, int w, int h);
void write_table(rgb *table, int table_size, FILE *file);
void colors_to_indices(rgb *pixels, rgb *table, int table_size, int w, int h);
int *indices(rgb *colors, rgb *table, int num_runs, int table_size);
rgb *read_table(int size);
void index_to_rgb(int *indices, rgb *table, int w, int h);

#endif /* _COLOR_TABLE_H_ */

