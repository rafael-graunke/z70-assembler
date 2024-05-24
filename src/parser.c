#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

char *strupper(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = toupper(str[i]);
    }
    return str;
}

HASH hash_instruction(char *instruction)
{
    int lastpos = strlen(instruction) - 1;
    return (instruction[0] << 8) + instruction[1] + instruction[lastpos];
}

uint8_t parse_operation(char *instruction, int *opcount)
{
    char *ins = strupper(instruction);
    HASH hash = hash_instruction(ins);

    switch (hash)
    {
    case ADD:
        *opcount = 2;
        return 0x00;
    case SUB:
        *opcount = 2;
        return 0x10;
    case CMP:
        *opcount = 2;
        return 0x20;
    case INC:
        *opcount = 1;
        return 0x30;
    case DEC:
        *opcount = 1;
        return 0x40;
    case AND:
        *opcount = 2;
        return 0x50;
    case OR:
        *opcount = 2;
        return 0x60;
    case NOT:
        *opcount = 1;
        return 0x70;
    case SHR:
        *opcount = 1;
        return 0x80;
    case SHL:
        *opcount = 1;
        return 0x90;
    case JMP:
        *opcount = 1;
        return 0xA0;
    case JZ:
        *opcount = 1;
        return 0xA1;
    case JS:
        *opcount = 1;
        return 0xA2;
    case JC:
        *opcount = 1;
        return 0xA3;
    case JO:
        *opcount = 1;
        return 0xA4;
    case JP:
        *opcount = 1;
        return 0xA5;
    case MOV:
        *opcount = 2;
        return 0xB0;
    case NOP:
        *opcount = 0;
        return 0xFF;
    }
}

uint8_t parse_single_address(char *operand)
{
    char *op = strupper(operand);
    switch (op[0])
    {
    case 'A':
        return 0;
        break;
    case 'B':
        return 1;
        break;
    case 'I':
        return 2;
        break;
    case '[':
        return 4;
        break;
    }
}

uint8_t parse_address(int opcount, char *operands, bool *has_imediate, uint8_t *imediate)
{
    if (opcount == 1)
        return parse_single_address(operands);

    // WIP
}