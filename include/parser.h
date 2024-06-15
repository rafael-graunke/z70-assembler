#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 20

// Hashed Instructions
#define ADD 0x4188
#define SUB 0x5397
#define CMP 0x439d
#define INC 0x4991
#define DEC 0x4488
#define AND 0x4192
#define OR 0x4fa4
#define NOT 0x4ea3
#define SHR 0x539a
#define SHL 0x5394
#define JMP 0x4a9d
#define JZ 0x4ab4
#define JS 0x4aa6
#define JC 0x4a86
#define JO 0x4a9e
#define JP 0x4aa0
#define MOV 0x4da5
#define NOP 0x4e9f

// Addresses
#define A 0x0
#define B 0x1
#define I 0x2
#define C 0x3
#define MI 0x4
#define M 0x5

typedef struct s_map
{
    char label[BUFFER_SIZE];
    int address;
} Map;

uint16_t hash_instruction(char *instruction);
uint8_t parse_operation(char *instruction, int *opcount);

uint8_t parse_constant(char *constant);
uint8_t parse_address(int opcount, char *operands, bool *has_imediate, uint8_t *imediate);

#endif