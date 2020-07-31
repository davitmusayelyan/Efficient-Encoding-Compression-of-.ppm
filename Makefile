.PHONY: clean

describe: formats.h describe.c
	clang -o describe -Wall describe.c -lm

ppm_p152v1: formats.h ppm_p152v1.c
	clang -o ppm_p152v1 -Wall ppm_p152v1.c -lm

152v1_ppm: formats.h 152v1_ppm.c
	clang -o 152v1_ppm -Wall 152v1_ppm.c -lm
