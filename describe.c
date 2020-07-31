#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "formats.h"

void skip_bytes(int n){
  for(int i=0; i<n; i++)
    getchar();
}

void print_date(){
  int date = read_big_endian(4);
  int year = date/10000;
  date = date%10000;
  int month = date/100;
  int day = date%100;
  fprintf(stdout, "Date: %d/%d/%d\n", month, day, year);
}

void print_time(){
  int time = read_big_endian(2);
  int hour = time/100;
  int minute = time%100;
  fprintf(stdout, "Time: %d.%d\n", hour, minute);
}

void print_dimensions(){
  fprintf(stdout, "Dimensions: %d(Width), %d(Height)\n",
          read_big_endian(2), read_big_endian(2));
}


void read_byte22(int table_size){
  int byte_value = getchar();
  //run-length vs raw data
  if((byte_value & 128) == 128)
    fprintf(stdout, "Run-length encoded image\n");
  else
    fprintf(stdout, "Raw-data image\n");
  
  //grayscale?
  if((byte_value & 64) == 64)
    fprintf(stdout, "The image is grayscale\n");
  else
    fprintf(stdout, "The image is not grayscale\n");
  
  if((byte_value & 32) == 32)
    fprintf(stdout, "Color table is present of size %d\n", table_size);
  else
    fprintf(stdout, "Color table is not present\n");
}


void print_string(){
  char c;
  while((c = getchar()) != '\0'){
    fprintf(stdout, "%c", c);
  }
  fprintf(stdout, "\n");
}

void describe(){
  skip_bytes(8);
  print_date();
  print_time();
  print_dimensions();
  read_byte22(read_big_endian(4));
  skip_bytes(3);
  print_string();
}

int main(){
  describe();
  return 0;
}
