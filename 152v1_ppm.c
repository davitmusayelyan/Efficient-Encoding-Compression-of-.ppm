#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "formats.h"
#include "encoding_type.c"
#include "color_table.h"

void skip_bytes(int n){
  for(int i=0; i<n; i++)
    getchar();
}


//checking if the image is grayscale
void byte22(int *run_length, int *grayscale, int *table){
  unsigned char byte_value = getchar();
  *run_length = (byte_value & 128) >> 7;
  *grayscale = (byte_value & 64) >> 6;
  *table = (byte_value & 32) >> 5;
}

//skipping the string
void skip_string(){
  while(getchar() != '\0')
    ;
}

int main(){
  skip_bytes(14);
  fprintf(stdout, "P3\n");
  
  int w = read_big_endian(2); //read width
  int h = read_big_endian(2); //read height
  int table_size = read_big_endian(4); //read table size
  
  fprintf(stdout, "%d %d\n", w, h);
  fprintf(stdout, "255\n");
  
  int run_length, grayscale, table;
  byte22(&run_length, &grayscale, &table); //read information from byte 22
  skip_bytes(3);
  skip_string(); //skip the string
  if(table == 0){
    skip_bytes(4);
  }
  else{
    rgb *table = read_table(table_size); //read the color table into the array
    if(run_length == 1){ //run-length and table
      int *indices = read_indices(w, h);
      index_to_rgb(indices, table, w, h);
      free(indices);
      return 0;
    }
    else{ //raw data and table
      int *indices = raw_indices(w, h);
      index_to_rgb(indices, table, w, h);
      free(indices);
      return 0;
    }
  }
  
  if(run_length == 0){ //no table, no run-length
    print_raw_data(w, h, grayscale);
    return 0;
  }
  else{
    print_runs(grayscale); //run-length, no table
  }
}
