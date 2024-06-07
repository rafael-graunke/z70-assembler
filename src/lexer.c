#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

static int state;
static int lexeme_size;
static char lexeme_buffer[MAX_LEXEME_SIZE];

void write_to_lexeme(char c)
{
    if (lexeme_size >= MAX_LEXEME_SIZE - 1)
    {
        printf("Lexeme buffer overflow.\n");
        exit(EXIT_FAILURE);
    }

    lexeme_buffer[lexeme_size] = c;
    lexeme_size++;
    lexeme_buffer[lexeme_size] = '\0';
}

enum token advance(FILE *f)
{
    state = START;
    lexeme_size = 0;
    memset(lexeme_buffer, 0, MAX_LEXEME_SIZE);

    int result;
    while ((result = step(f)) == 0)
        ;

    if (result == EOF)
        return _EOF;

    switch (result)
    {
    case LABEL:
        printf("LABEL(%s)\n", lexeme_buffer);
        break;
    case INSTRUCTION:
        printf("INSTRUCTION(%s)\n", lexeme_buffer);
        break;
    case A_REGISTER:
        printf("REGISTER(A)\n");
        break;
    case B_REGISTER:
        printf("REGISTER(B)\n");
        break;
    case I_REGISTER:
        printf("REGISTER(I)\n");
        break;
    case CONSTANT:
        printf("CONSTANT(%s)\n", lexeme_buffer);
        break;
    case OPEN_ADDRESS:
        printf("OPEN_ADDRESS\n");
        break;
    case CLOSE_ADDRESS:
        printf("CLOSE_ADDRESS\n");
        break;
    case COMMA:
        printf("COMMA\n");
        break;
    }

    return result;
}

int step(FILE *f)
{
    char cursor = toupper(getc(f));
    switch (state)
    {
    case START:
        if (cursor == EOF)
            return EOF;

        if (isspace(cursor))
            break;

        switch (cursor)
        {
        case 'A':
        case 'B':
        case 'I':
            char next = toupper(getc(f));
            fseek(f, -1, SEEK_CUR);
            if (!isalpha(next))
            {
                switch (cursor)
                {
                case 'A':
                    return A_REGISTER;
                case 'B':
                    return B_REGISTER;
                case 'I':
                    return I_REGISTER;
                }
            }
            write_to_lexeme(cursor);
            state = READING;
            break;
        case '[':
            return OPEN_ADDRESS;
        case ']':
            return CLOSE_ADDRESS;
        case ',':
            return COMMA;
        default:
            if (isalpha(cursor))
            {
                write_to_lexeme(cursor);
                state = READING;
                break;
            }
            if (isdigit(cursor))
            {
                write_to_lexeme(cursor);
                if (isdigit((cursor = toupper(getc(f)))))
                {
                    write_to_lexeme(cursor);
                    if ((cursor = toupper(getc(f))) == 'H')
                    {
                        write_to_lexeme(cursor);
                        return CONSTANT;
                    }
                }
                printf("Unexpected constant formatting %c.\n", cursor);
                exit(EXIT_FAILURE);
                break;
            }
            printf("Unexpected character %c.\n", cursor);
            exit(EXIT_FAILURE);
        }
        break;
    case READING:
        if (isalpha(cursor))
        {
            write_to_lexeme(cursor);
            break;
        }
        else if (isspace(cursor))
        {
            return INSTRUCTION;
        }
        else if (cursor == EOF)
        {
            return INSTRUCTION;
        }
        else if (cursor == ':')
        {
            return LABEL;
        }
        printf("Unexpected character %c.\n", cursor);
        exit(EXIT_FAILURE);
    }

    return 0;
}