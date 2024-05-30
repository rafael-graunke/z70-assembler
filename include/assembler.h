#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

#include <stdio.h>
#include "parser.h"

// Assembling States
#define START 0
#define FIRST_STRING 1
#define LABEL 2
#define OPERATION 3
#define ADDRESS 4
#define WRITE 5
#define RESET 0xF0
#define FINISHED 0xFF

int run(FILE *source, FILE *target);
int assemble(FILE *source, FILE *target, Map *labels, int *lsize);

#endif