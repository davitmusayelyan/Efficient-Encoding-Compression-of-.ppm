#include <stdio.h>
#include <stdlib.h>
#include "formats.h"
#include "encoding_type.h"
#include "color_table.c"

//******************RUN-LENGTH ENCODING WITH & WITHOUT A COLOR TABLE**********************

//finding the number of runs
int num_runs(rgb *pixels, int w, int h){
  rgb *curr = pixels;
  int runs = 1;
  for(int i=1; i<w*h; i++){
    if(curr->r == pixels[i].r &&
       curr->g == pixels[i].g &&
       curr->b == pixels[i].b)
      curr = pixels+i;
    else{
      runs++;
      curr = pixels+i;
    }
  }
  return runs;
}

//creating the arrays of run lengths and colors in a corresponding order
void run_length_encoding(int w, int h, rgb *pixels, int *run_lengths, rgb *colors){
  rgb curr = pixels[0];
  int run_length = 1;
  int j=0;
  for(int i=1; i<w*h; i++){
    if(curr.r == pixels[i].r &&
       curr.g == pixels[i].g &&
       curr.b == pixels[i].b)
    {
      run_length++;
      curr = pixels[i];
    }
    else{
      run_lengths[j]=run_length;
      run_length = 1;
      colors[j]=curr;
      j++;
      curr = pixels[i];
    }
  }
  colors[j] = curr;
  run_lengths[j] = run_length;
}

//writing bytes of run lengths and colors
void bytes_encoding(int num_runs, int *run_lengths, rgb *colors, int grayscale, FILE *file){
  big_endian_int(4, num_runs, file); //4-byte big-endian int for number of runs
  for(int i = 0; i<num_runs; i++){
    big_endian_int(4, run_lengths[i], file);
  }
  if(grayscale == 0){
    for(int i = 0; i<num_runs; i++){
      putc(colors[i].r, file);
      putc(colors[i].g, file);
      putc(colors[i].b, file);
    }
  }
  else
    for(int i = 0; i<num_runs; i++){
      putc(colors[i].r, file);
    }
}

//encoding of bytes with color table
void bytes_encoding_t(int num_runs, int *run_lengths, rgb *colors, rgb *table, int size, FILE *file){
  big_endian_int(4, num_runs, file); //4-byte big-endian int for number of runs
  for(int i = 0; i<num_runs; i++){
    big_endian_int(4, run_lengths[i], file);
  }
  int *color_indices = indices(colors, table, num_runs, size);
  for(int i=0; i<num_runs; i++){
    putc(color_indices[i], file);
  }
  free(color_indices);
  return;
}

void write_runs(rgb *pixels, int w, int h, int grayscale, FILE *file, char *table_size, rgb *table){
  int n_runs = num_runs(pixels, w, h);
  int *run_lengths = malloc(sizeof(int)*n_runs);
  rgb *colors = malloc(sizeof(rgb)*n_runs);
  run_length_encoding(w, h, pixels, run_lengths, colors);
  if(table_size  == NULL)
    bytes_encoding(n_runs, run_lengths, colors, grayscale, file); //runs, runlengths and colors
  else
    bytes_encoding_t(n_runs, run_lengths, colors, table, atoi(table_size), file);
  free(run_lengths);
  free(colors); 
}


//***************************************RAW-DATA*************************************

void raw_data(rgb *pixels, int w, int h, int grayscale, FILE *file, char *table_size, rgb *table){
  if(grayscale == 0 && table_size == NULL){
    for(int i = 0; i<w*h; i++){
      putc(pixels[i].r, file);
      putc(pixels[i].g, file);
      putc(pixels[i].b, file);
    }
  } else if(table_size!=NULL){
    for(int i = 0; i<w*h; i++){
      putc(closest_rgb(pixels[i], table, atoi(table_size)), file);
    }
  }
  else
    for(int i = 0; i<w*h; i++){
      putc(pixels[i].r, file);
    }
}

//********************************* READING RUNS *********************************

//reading the run-lengths and colors
void print_runs(int grayscale){
  int num_runs = read_big_endian(4);
  int run_lengths[num_runs];
  rgb colors[num_runs];
  for(int i=0; i<num_runs; i++){
    run_lengths[i] = read_big_endian(4);
  }
  if(grayscale == 0){
    for(int i=0; i<num_runs; i++){
      colors[i].r = getchar();;
      colors[i].g = getchar();
      colors[i].b = getchar();
    }
  }
  else
    for(int i=0; i<num_runs; i++){
      int pixel = getchar();
      colors[i].r = pixel;
      colors[i].g = pixel;
      colors[i].b = pixel;
    }
    for(int i=0; i<num_runs; i++){
      for(int j = 0; j<run_lengths[i]; j++)
        fprintf(stdout, "%d %d %d\n", colors[i].r, colors[i].g, colors[i].b);
    }
}

//****************************READING RUNS WITH COLOR TABLE***************************

//from run-length encoded color table indices to raw list of the indices
int *read_indices(int w, int h){
  int size = read_big_endian(4); //read the number of runs
  int run_lengths[size];
  int item_indices[size];
  for(int i=0; i<size; i++){
    run_lengths[i] = read_big_endian(4);
  }
  for(int i=0; i<size; i++)
    item_indices[i] = getchar();
  int *indices = malloc(sizeof(int)*w*h);
  int sum=0;
  for(int i=0; i<size; i++){
    for(int j=0; j<run_lengths[i]; j++){
      indices[sum+j] = item_indices[i];
    }
    sum += run_lengths[i];
  }
  return indices;
}

//*************************READING RAW-DATA WITH COLOR TABLE***************************
int *raw_indices(int w, int h){
  int *indices = malloc(sizeof(int)*w*h);
  for(int i=0; i<w*h; i++){
    indices[i] = getchar();
  }
  return indices;
} 

  
//*************************************PRINTING RAW-DATA*******************************
void print_raw_data(int w, int h, int grayscale){
  for(int i=0; i<w*h; i++){
    if(grayscale == 0)
      fprintf(stdout, "%d %d %d\n", getchar(), getchar(), getchar());
    else{
      int rgb_component = getchar();
      fprintf(stdout, "%d %d %d\n", rgb_component, rgb_component, rgb_component);
    }
  }
}

