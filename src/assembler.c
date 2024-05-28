#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "assembler.h"
#include "parser.h"

int assemble(FILE *source, FILE *target)
{
    int opcount, lsize = 0, state = START, iterations = 2;
    bool running = true, has_imediate = false, jmp = false;
    char curr, buffer[BUFFER_SIZE] = {0};
    uint8_t operation, addressing, imediate;
    Map labels[255];

    for (int iter = 0; iter < iterations; iter++)
    {
        running = true;
        state = START;
        fseek(source, 0, SEEK_SET);
        fseek(target, 0, SEEK_SET);
        while (running)
        {
            switch (state)
            {
            case START:
                while ((curr = fgetc(source)) == ' ')
                    continue;
                strncat(buffer, &curr, 1);
                state = FIRST_STRING;
                break;

            case FIRST_STRING:
                switch (curr = fgetc(source))
                {
                case ':':
                    state = LABEL;
                    continue;
                case ' ':
                case '\n':
                case EOF:
                    state = OPERATION;
                    continue;
                }
                strncat(buffer, &curr, 1);
                break;

            case LABEL:
                strcpy(labels[lsize].label, buffer);
                labels[lsize].address = ftell(target);
                lsize++;
                memset(buffer, 0, BUFFER_SIZE);
                while ((curr = getc(source)) != '\n')
                    curr = getc(source);
                state = START;
                break;

            case OPERATION:
                operation = parse_operation(buffer, &opcount);
                memset(buffer, 0, BUFFER_SIZE);
                state = ADDRESS;
                jmp = (operation >= 0xA0 && operation <= 0xA5);
                if (opcount == 0)
                    state = WRITE;
                break;

            case ADDRESS:
                while ((curr = fgetc(source)) != '\n')
                {
                    strncat(buffer, &curr, 1);
                    continue;
                }
                if (!jmp && (addressing = parse_address(opcount, buffer, &has_imediate, &imediate)) == 0XFF)
                    return EXIT_FAILURE;
                state = WRITE;

            case WRITE:
                if (operation == 0xFF)
                {
                    running = false;
                    fwrite(&operation, sizeof(operation), 1, target);
                    continue;
                }

                if (jmp)
                {
                    fwrite(&operation, sizeof(operation), 1, target);
                    uint8_t addr = 0x00;
                    for (int i = 0; i < lsize; i++)
                    {
                        if (strcmp(labels[i].label, buffer) == 0)
                        {
                            addr = labels[i].address;
                            break;
                        }
                    }
                    fwrite(&addr, sizeof(addr), 1, target);
                    state = RESET;
                    continue;
                }

                uint8_t instruction = operation + addressing;
                fwrite(&instruction, sizeof(instruction), 1, target);

                if (has_imediate)
                    fwrite(&imediate, sizeof(imediate), 1, target);

                state = RESET;

            case RESET:
                state = START;
                jmp = false;
                has_imediate = false;
                memset(buffer, 0, BUFFER_SIZE);
                break;
            }
        }
    }
    return EXIT_SUCCESS;
}