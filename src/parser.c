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
    strupper(instruction);
    HASH hash = hash_instruction(instruction);

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
    uint8_t f, s;
    char first, second;
    first = toupper(constant[0]);
    second = toupper(constant[1]);

    f = (first - '0') << 4;
    if (first >= 'A' && first <= 'F')
        f = (first - 'A' + 0xA) << 4;

    s = second - '0';
    if (second >= 'A' && second <= 'F')
        s = second - 'A' + 0xA;

    return f + s;
}

uint8_t parse_single_address(char *operand)
{
    int length = strlen(operand);
    switch (length)
    {
    case 1:
        switch (*operand)
        {
        case 'A':
            return A;
        case 'B':
            return B;
        case 'I':
            return I;
        default:
            return 0xFF;
        }
    case 3:
        if (operand[1] == 'I' && operand[0] == '[' && operand[2] == ']')
            return MI;
        return 0xFF;
    default:
        return 0xFF;
    }
}

uint8_t parse_operand_address(char *operand, bool *has_imediate, uint8_t *imediate)
{
    int length = strlen(operand);
    char constant[2];
    switch (length)
    {
    case 1:
        switch (*operand)
        {
        case 'A':
            return A;
        case 'B':
            return B;
        case 'I':
            return I;
        default:
            return 0xFF;
        }
        break;
    case 3:
        if (operand[1] == 'I' && operand[0] == '[' && operand[2] == ']')
            return MI;

        if (operand[2] == 'H')
        {
            strncpy(constant, operand, 2);
            *has_imediate = true;
            *imediate = parse_constant(constant);
            return C;
        }

        return 0XFF;
    case 5:
        if (operand[3] == 'H' && operand[0] == '[' && operand[4] == ']')
        {
            strncpy(constant, ++operand, 2);
            *has_imediate = true;
            *imediate = parse_constant(constant);
            return M;
        }
        return 0xFF;
    default:
        return 0xFF;
    }
}

uint8_t parse_address(int opcount, char *operands, bool *has_imediate, uint8_t *imediate)
{
    strupper(operands);
    if (opcount == 1)
        return parse_single_address(operands);

    /*This matrix allows mapping the individual operand address
    to their combination e.g A = 0 and B = 1, then lookup[0][1]
    should have the correct value for the addressing mode.*/
    uint8_t lookup[6][6] = {
        {0xFF, 0x0, 0x2, 0x6, 0x4, 0xA},
        {0x1, 0xFF, 0xFF, 0x7, 0xFF, 0xB},
        {0x3, 0xFF, 0xFF, 0x8, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0x5, 0xFF, 0xFF, 0x9, 0xFF, 0xFF},
        {0xC, 0xD, 0xFF, 0xFF, 0xFF, 0xFF}};

    char curr;
    char buffer[BUFFER_SIZE] = {0};
    int length = strlen(operands);
    bool reached_second = false;
    uint8_t from, to;

    for (int i = 0; i < length; i++)
    {
        curr = operands[i];

        if (curr == ' ')
            continue;

        if (curr == ',')
        {
            reached_second = true;
            if ((from = parse_operand_address(buffer, has_imediate, imediate)) == 0xFF)
                return from;
            memset(buffer, 0, BUFFER_SIZE);
            continue;
        }
        strncat(buffer, &curr, 1);
    }
    if ((to = parse_operand_address(buffer, has_imediate, imediate)) == 0xFF)
        return to;

    return lookup[from][to];
}