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

uint8_t parse_constant(char *constant)
{
    uint8_t c = 0x00;
    char first, second;
    first = toupper(constant[0]);
    second = toupper(constant[1]);

    if (first > 'A' && first < 'F')
        c += (first - 'A' + 0xA) << 4;
    else
        c += (first - '0') << 4;

    if (second > 'A' && second < 'F')
        c += (second - 'A' + 0xA);
    else
        c += (second - '0');

    return c;
}

uint8_t parse_single_address(char *operand)
{
    char *op = strupper(operand);
    switch (op[0])
    {
    case 'A':
        return 0;
    case 'B':
        return 1;
    case 'I':
        return 2;
    case '[':
        return 4;
    }
}

uint8_t parse_operand_address(char *operand, bool *has_imediate, uint8_t *imediate)
{
    char *op = strupper(operand);
    char constant[2];
    switch (op[0])
    {
    case 'A':
        return A;
    case 'B':
        return B;
    case 'I':
        return I;
    case '[':
        if (op[1] != 'I')
        {
            *has_imediate = true;
            constant[0] = op[1];
            constant[1] = op[2];
            *imediate = parse_constant(constant);
            return M;
        }
        return MI;
    default:
        *has_imediate = true;
        strncpy(constant, op, 2);
        *imediate = parse_constant(constant);
        return C;
    }
}

uint8_t parse_address(int opcount, char *operands, bool *has_imediate, uint8_t *imediate)
{

    /*This matrix allows mapping the individual operand address
    to their combination e.g A = 0 and B = 1, then lookup[0][1]
    should have the correct value for the addressing mode.*/
    uint8_t lookup[6][6] = {
        {0xFF, 0x0, 0x2, 0x6, 0xA, 0x4},
        {0x1, 0xFF, 0xFF, 0x7, 0xB, 0xFF},
        {0x3, 0xFF, 0xFF, 0x8, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xC, 0xD, 0xFF, 0xFF, 0xFF, 0xFF},
        {0x5, 0xFF, 0xFF, 0x9, 0xFF, 0xFF}};

    if (opcount == 1)
        return parse_single_address(operands);

    char curr;
    bool reached_second = false;
    char buffer[BUFFER_SIZE] = {0};
    uint8_t from, to;
    int length = strlen(operands);
    for (int i = 0; i < length; i++)
    {
        curr = operands[i];

        if (curr == ' ')
            continue;

        if (curr == ',')
        {
            reached_second = true;
            from = parse_operand_address(buffer, has_imediate, imediate);
            memset(buffer, 0, BUFFER_SIZE);
            continue;
        }
        strncat(buffer, &curr, 1);
    }
    to = parse_operand_address(buffer, has_imediate, imediate);

    return lookup[from][to];
}