#ifndef _FORMATS_H_
#define _FORMATS_H_

struct rgb{
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

typedef struct rgb rgb;

void describe();
void ppm_p152v1();
void p152v1_ppm();

//writes the value as big-endian integer of n bytes
void big_endian_int(int n, int value, FILE *file){
  for(int j = n; j>=1; j--){
    int byte_value = 0;
    for(int i=8*j-1; i>=(j-1)*8; i--){
      if(value >= pow(2, i)){
        byte_value += pow(2, i-(j-1)*8);
        value -=pow(2, i);
      }
    }
    putc(byte_value, file);
  }
  return;
}

//reading big-endian integers of given bytes
int read_big_endian(int bytes){
  int value = 0;
  for(int i=bytes-1; i>=0; i--){
    value += pow(2, 8*i)*getchar();
  }
  return value;
}
#endif /* _FORMATS_H_ */
