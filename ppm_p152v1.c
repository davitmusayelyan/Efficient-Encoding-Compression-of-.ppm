#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "formats.h"
#include "encoding_type.c"
#include "color_table.h"

//write 0 bytes to the given file
void skip_bytes(int n, FILE *file){
  for(int i=0; i<n; i++)
    putc(0, file);
}

//writes the format name as 6 bytes into the file
void bytes0_5(FILE *file){
  char format_name[6] = "P152v1";
  for(int i=0; i<6; i++){
    putc(format_name[i], file);
  }
  return;
}

//to be written later
void bytes6_7(FILE *file){
  skip_bytes(2, file);
  return;
}
  
//gives today's date in the format YYYYMMDD
int date(){
  time_t current;
  struct tm *local;
  time(&current);
  local = localtime(&current);
  int date = (local->tm_year+1900) * 10000 + (local->tm_mon+1)*100+ local->tm_mday;
  return date;
}

//gives current time in the format HHMM
int curr_time(){
  time_t current;
  struct tm *local;
  time(&current);
  local = localtime(&current);
  int time = local->tm_hour*100 + local->tm_min;
  return time;
}

//writes today's date as a 4-byte big-endian integer
void bytes8_11(FILE *file){
  int todays_date = date();
  big_endian_int(4, todays_date, file);
  return;
}

//writes current time as a 2-byte big-endian integer
void bytes12_13(FILE *file){
  int time = curr_time();
  big_endian_int(2, time, file);
  return;
}

//Checking if the given file is P3; obtaining the width/height information
void header(char *format_num, int *w, int *h, FILE *file){
  int d;
  if (fscanf(file,"P%c\n%d %d", format_num, w, h)!=3) {
    fprintf(stderr, "Was not able to store the data");
  }
  if (fscanf(file,"%d",&d)!=1) {
    fprintf(stderr, "Was not able to store the data");
  } else if (d!=255) {
    fprintf(stderr, "The color depth is not 255");
  }
}

//writing the width and the height in 2-byte big-endian int
void bytes14_17(int w, int h, FILE *file){
  big_endian_int(2, w, file);
  big_endian_int(2, h, file);
}

//for future extension
void bytes18_21(int size, FILE *file){
  big_endian_int(4, size, file);
  return;
}

//places the pixels to an array by also checking if the image is grayscale
rgb *array_p3(int w, int h, int *grayscale, FILE *file){
  rgb *pixels = malloc(sizeof(rgb)*w*h);
  for(int i=0; i<w*h; i++){
    if(fscanf(file, "%hhu %hhu %hhu\n", &pixels[i].r, &pixels[i].g, &pixels[i].b) !=3)
      fprintf(stderr, "ERROR\n");
    //checking if it is grayscale
    if(pixels[i].r != pixels[i].g || pixels[i].r != pixels[i].b){
      *grayscale=0;
    }
  }
  fclose(file);
  return pixels;
}

//places the pixels to an array by also checking if the image is grayscale
rgb *array_p6(int w, int h, int *grayscale, FILE *file){
  char c;
  for(int i=0; i<3; i++) //P6, width & height, depth
    while((c=getc(file)) == '\n');
  rgb *pixels = malloc(sizeof(rgb)*w*h);
  for(int i=0; i<w*h; i++){
    pixels[i].r = getc(file);
    pixels[i].g = getc(file);
    pixels[i].b = getc(file);
    //checking if it is grayscale
    if(pixels[i].r != pixels[i].g || pixels[i].r != pixels[i].b){
      *grayscale=0;
    }
  }
  fclose(file);
  return pixels;
}

//(run-for-length or raw colors)?, (grayscale)?, (color table)?
void byte22(int run_length, int grayscale, char *table_size, FILE *file){
  if(grayscale == 1)
    table_size = NULL;
  int byte_ = run_length << 7; //run_length
  byte_ = byte_ | (grayscale << 6);
  byte_ = byte_ | ((table_size == NULL ? 0 : 1) << 5);
  putc(byte_, file);
}

//for future extension
void bytes23_25(FILE *file){
  skip_bytes(3, file);
}

//string
void bytes26_(char *text, FILE *file){
  int i = 0;
  while(text[i] != '\0')
  {
    putc(text[i], file);
    i++;
  }
  putc('\0', file);
}

int main(int argc, char *argv[]){
  //command line processing
  int opt;
  char *input = NULL;
  char *output = NULL;
  char *table_size = NULL;
  int size = 0;
  int run_length = 0;
  int string = *argv[1] == '-' ? 0 : 1; //whether there is a text
  if(string == 1)
    optind = 2;
  while((opt = getopt(argc, argv, "i:o:t:r")) != -1){
    switch(opt){
    case 'i':
      input = optarg;
      break;
    case 'o':
      output = optarg;
      break;
    case 't':
      table_size = optarg;
      break;
    case 'r':
      run_length = 1;
      break;
    case '?':
      fprintf(stderr, "Bad command input");
    }
    string = 0;
  }
  
  FILE *input_file = (input == NULL ? stdin : fopen(input, "r")); //input file
  FILE *output_file = (output == NULL ? stdout : fopen(output, "w")); //input file
  if(table_size!=NULL){
    size = atoi(table_size);
  }
  
  //bytes
  bytes0_5(output_file); // format name
  bytes6_7(output_file); // for future extension
  bytes8_11(output_file); // date in 4-byte big-endian int
  bytes12_13(output_file); // time in 2-byte big-endian int
  
  int w, h;
  char format_num;
  header(&format_num, &w,&h, input_file);
  bytes14_17(w, h, output_file); // width and height in 2-byte big-endian int
  bytes18_21(size, output_file); // color table size
  int grayscale = 1;
  rgb *pixels = (format_num == '3' ? array_p3(w, h, &grayscale, input_file) :
                   array_p6(w, h, &grayscale, input_file));

  //grayscale, run-length or raw colors, 6 empty bytes
  byte22(run_length, grayscale, table_size, output_file);
  bytes23_25(output_file); // for future extension
  
  if(string == 0) //string
    putc('\0', output_file);
  else
    bytes26_(argv[1], output_file);
  
  rgb *table = NULL;
  //color-table
  if(size == 0 || grayscale == 1)
    skip_bytes(4, output_file);
  else
    if(grayscale != 1){
      table = color_table(pixels, size, w, h);
      write_table(table, size, output_file);
      colors_to_indices(pixels, table, size, w, h);
    }
  
  //performing the appropriate encoding 
  if(run_length == 1) 
    write_runs(pixels, w, h, grayscale, output_file, table_size, table);
  else
    raw_data(pixels, w, h, grayscale, output_file, table_size, table);

  free(pixels);
  free(table);
  
  return 0;
}
