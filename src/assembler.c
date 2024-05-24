#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "assembler.h"
#include "parser.h"

#define SKIP_START 0
#define FIRST_STRING 1
#define TAG_FOUND 2
#define OPERATION 3
#define ADDRESS 4
#define RESET 0xFF

int assemble(FILE *source, FILE *target)
{
    int state = SKIP_START;
    bool running = true;
    char buffer[MAX_BUFF_SIZE] = {0};
    char curr;
    int opcount;
    uint8_t operation;
    uint8_t addressing;
    uint8_t imediate;
    bool has_imediate = false;

    while (running)
    {
        switch (state)
        {
        case SKIP_START:
            curr = fgetc(source);

            if (curr == ' ' || curr == '\t')
                continue;

            strncat(buffer, &curr, 1);
            state = FIRST_STRING;
            break;

        case FIRST_STRING:
            curr = fgetc(source);

            if (curr == ':')
            {
                state = TAG_FOUND;
                continue;
            }

            if (curr == ' ')
            {
                state = OPERATION;
                continue;
            }

            if (curr == '\n' || curr == EOF)
            {
                state = OPERATION;
                continue;
            }

            strncat(buffer, &curr, 1);
            break;

        case TAG_FOUND:
            break;

        case OPERATION:
            operation = parse_operation(buffer, &opcount);
            memset(buffer, 0, MAX_BUFF_SIZE);

            if (opcount == 0)
            {
                state = RESET;
                continue;
            }

            state = ADDRESS;
            break;

        case ADDRESS:
            curr = fgetc(source);

            if (curr == '\n')
            {
                addressing = parse_address(opcount, buffer, &has_imediate, &imediate);
                state = RESET;
            }

            strncat(buffer, &curr, 1);
            break;

        case RESET:
            if (operation == 0xFF)
            {
                running = false;
                fwrite(&operation, sizeof(operation), 1, target);
                continue;
            }

            uint8_t instruction = operation + addressing;
            fwrite(&instruction, sizeof(instruction), 1, target);

            if (has_imediate)
                fwrite(&has_imediate, sizeof(has_imediate), 1, target);

            state = SKIP_START;
            memset(buffer, 0, MAX_BUFF_SIZE);
            break;
        }
    }

    return EXIT_SUCCESS;
}