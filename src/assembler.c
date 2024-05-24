#include <string.h>
#include "assembler.h"
#include "parser.h"

#define SKIP_START 0
#define FIRST_STRING 1
#define TAG_FOUND 2
#define INSTRUCTION 3

int assemble(FILE *source, FILE *target)
{
    int state = SKIP_START;
    char buffer[MAX_BUFF_SIZE] = {0};
    char tag[MAX_BUFF_SIZE];
    char instruction[MAX_BUFF_SIZE];
    char foperator[MAX_BUFF_SIZE];
    char soperator[MAX_BUFF_SIZE];

    char curr;

    while ((curr = fgetc(source)))
    {
        switch (state)
        {
        case SKIP_START:
            if (curr == ' ' || curr == '\t')
                continue;

            strncat(buffer, &curr, 1);
            state = FIRST_STRING;
            break;
        case FIRST_STRING:
            if (curr == ':')
            {
                strncpy(tag, buffer, MAX_BUFF_SIZE);
                memset(buffer, 0, MAX_BUFF_SIZE);
                state = TAG_FOUND;
                continue;
            }

            if (curr == ' ')
            {
                strncpy(instruction, buffer, MAX_BUFF_SIZE);
                memset(buffer, 0, MAX_BUFF_SIZE);
                state = INSTRUCTION;
                continue;
            }

            strncat(buffer, &curr, 1);
            break;

        case TAG_FOUND:
            break;

        case INSTRUCTION:
            int operators = get_op_count(instruction);
            break;
        }
    }
}