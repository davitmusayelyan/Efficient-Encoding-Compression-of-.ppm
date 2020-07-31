#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "formats.h"
#include "color_table.h"

//*******************************COLOR_TABLE*******************************

//chooses initial colors for the color table with the given size
rgb *initial_table(int table_size){
  rgb *table = malloc(sizeof(rgb)*table_size);
  for(int i=0; i< table_size; i++){
    int rgb_value = 255*i/table_size;
    rgb color = {rgb_value, rgb_value, rgb_value};
    table[i] = color;
  }
  return table;
}

//calculates the euclidean distance between two pixels
int distance(rgb pixel1, rgb pixel2){
  int d = sqrt(pow(pixel1.r-pixel2.r, 2)+
               pow(pixel1.g-pixel2.g, 2)+
               pow(pixel1.b-pixel2.b, 2));
  return d;
}

//determines the closest color-table color to the given color
//and gives its index
int closest_rgb(rgb pixel, rgb *table, int table_size){
  int min = distance(pixel, table[0]);
  int min_index = 0;
  int d;
  for(int i=1; i<table_size; i++){
    if(min > (d = distance(pixel, table[i]))){
      min = d;
      min_index = i;  
    }
  }
  return min_index;
}

//checks if two colors are the same
int same_color(rgb pixel1, rgb pixel2){
  if(pixel1.r == pixel2.r &&
     pixel1.g == pixel2.g &&
     pixel1.b == pixel2.b)
    return 1;
  return 0;
}

//calculates the new color-table colors which are the averages of the closest colors
//reports if there was an improvement or no
int mean_improvement(rgb *table, rgb *pixels, int w, int h, int table_size){
  int improvement = 0;
  cluster_mean_info clusters[table_size];
  for(int i = 0; i< table_size; i++){
    cluster_mean_info mean_rgb = {0, 0, 0, 0}; 
    clusters[i]= mean_rgb;
  }
  for(int i=0; i<w*h; i++){
    int color_index = closest_rgb(pixels[i], table, table_size);
    clusters[color_index].r_squares_sum += pow(pixels[i].r, 2);
    clusters[color_index].g_squares_sum += pow(pixels[i].g, 2);
    clusters[color_index].b_squares_sum += pow(pixels[i].b, 2);
    clusters[color_index].num_colors ++;
  }
  for(int i=0; i<table_size; i++){
    //formula for averaging rgb colors is summing up the squares of each rgb component,
    //dividing it by the number of colors, taking the square root
    rgb new_mean;
    if(clusters[i].num_colors!=0){
      new_mean.r = (unsigned char)sqrt(clusters[i].r_squares_sum/clusters[i].num_colors);
      new_mean.g = (unsigned char)sqrt(clusters[i].g_squares_sum/clusters[i].num_colors);
      new_mean.b = (unsigned char)sqrt(clusters[i].b_squares_sum/clusters[i].num_colors);
    }
    else{
      new_mean = table[i];
    }
    if(same_color(new_mean, table[i])!=1){
      improvement = 1;
      table[i] = new_mean;
    }
  }
  return improvement;
}

//while there is an improvement, updates the table
rgb *color_table(rgb *pixels, int table_size, int w, int h){
  rgb *table = initial_table(table_size);
  while(mean_improvement(table, pixels, w, h, table_size) != 1){
    
  }
  return table;
}

//writes the table to the given file
void write_table(rgb *table, int table_size, FILE *file){
  for(int i=0; i< table_size; i++){
    putc(table[i].r, file);
    putc(table[i].g, file);
    putc(table[i].b, file);
  }
}

//turns colors to their closest color-table color and returns
//the updated array of the colors
void colors_to_indices(rgb *pixels, rgb *table, int table_size, int w, int h){
  for(int i=0; i< w*h; i++){
    int index = closest_rgb(pixels[i], table, table_size);
    pixels[i] = table[index];
  }
}

//creates an array of indices corresponding to the colors
int *indices(rgb *colors, rgb *table, int num_runs, int table_size){
  int *indices = malloc(sizeof(int)*num_runs);
  for(int i=0; i<num_runs; i++){
    int index = closest_rgb(colors[i], table, table_size);
    indices[i]=index;
  }
  return indices;
}

//*******************************READING DATA WITH COLOR_TABLE*******************************
//read the size of the table
int read_size(){
  return read_big_endian(4);
}

//read and write the table to an array
rgb *read_table(int size){
  rgb *table = malloc(sizeof(rgb)*size);
  for(int i=0; i<size; i++){
    table[i].r = getchar();
    table[i].g = getchar();
    table[i].b = getchar();
  }
  return table;
}

//given the colors with indices in a row and the color table prints rgbs
void index_to_rgb(int *indices, rgb *table, int w, int h){
  for(int i=0; i<w*h; i++){
    fprintf(stdout, "%d %d %d\n", table[indices[i]].r, table[indices[i]].g, table[indices[i]].b);
  }
}