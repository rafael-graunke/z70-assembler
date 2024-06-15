#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hashmap.h"
#include "lexer.h"

typedef struct hm_value
{
    int value;
    int op_count;
} Mnemonic;

Mnemonic *create_mn(int value, int op_count)
{
    Mnemonic *mn = (Mnemonic *)malloc(sizeof(Mnemonic));
    mn->value = value;
    mn->op_count = op_count;

    return mn;
}

HashMap *init_symbols(void)
{
    HashMap *hashmap = hm_create(18);
    hm_insert(hashmap, "ADD", create_mn(0x00, 2));
    hm_insert(hashmap, "SUB", create_mn(0x10, 2));
    hm_insert(hashmap, "CMP", create_mn(0x20, 2));
    hm_insert(hashmap, "INC", create_mn(0x30, 1));
    hm_insert(hashmap, "DEC", create_mn(0x40, 1));
    hm_insert(hashmap, "AND", create_mn(0x50, 2));
    hm_insert(hashmap, "OR", create_mn(0x60, 2));
    hm_insert(hashmap, "NOT", create_mn(0x70, 1));
    hm_insert(hashmap, "SHR", create_mn(0x80, 1));
    hm_insert(hashmap, "SHL", create_mn(0x90, 1));
    hm_insert(hashmap, "JMP", create_mn(0xA0, 1));
    hm_insert(hashmap, "JZ", create_mn(0xA1, 1));
    hm_insert(hashmap, "JS", create_mn(0xA2, 1));
    hm_insert(hashmap, "JC", create_mn(0xA3, 1));
    hm_insert(hashmap, "JO", create_mn(0xA4, 1));
    hm_insert(hashmap, "JP", create_mn(0xA5, 1));
    hm_insert(hashmap, "MOV", create_mn(0xB0, 2));
    hm_insert(hashmap, "NOP", create_mn(0xFF, 0));

    return hashmap;
}

void reset_lexer(Lexer *lexer)
{
    lexer->state = START;
    lexer->lexeme_size = 0;
    memset(lexer->lexeme_buffer, 0, MAX_LEXEME_SIZE);
}

Lexer *create_lexer(FILE *f)
{
    Lexer *lexer = (Lexer *)malloc(sizeof(Lexer));
    lexer->file = f;
    lexer->symbols = init_symbols();
    reset_lexer(lexer);

    return lexer;
}

void destroy_lexer(Lexer *lexer)
{
    hm_destroy(lexer->symbols);
    fclose(lexer->file);
    free(lexer);
}

void write_to_lexeme(Lexer *lexer, char c)
{
    if (lexer->lexeme_size >= MAX_LEXEME_SIZE - 1)
    {
        printf("Lexeme buffer overflow.\n");
        exit(EXIT_FAILURE);
    }

    lexer->lexeme_buffer[lexer->lexeme_size] = c;
    lexer->lexeme_size++;
    lexer->lexeme_buffer[lexer->lexeme_size] = '\0';
}

Token advance(Lexer *lexer)
{
    reset_lexer(lexer);
    int result;
    while ((result = step(lexer)) == 0)
        ;

    Token token;
    token.type = result;

    if (result == _EOF)
        return token;

    switch (result)
    {
    case LABEL:
    case MNEMONIC:
    case CONSTANT:
        strncpy(token.lexval, lexer->lexeme_buffer, MAX_LEXEME_SIZE);
        break;
    }

    return token;
}

int step(Lexer *lexer)
{
    char cursor = toupper(getc(lexer->file));
    switch (lexer->state)
    {
    case START:
        if (cursor == EOF)
            return _EOF;

        if (cursor == '\n')
            return EOL;

        if (isspace(cursor))
            break;

        switch (cursor)
        {
        case 'A':
        case 'B':
        case 'I':
            char next = toupper(getc(lexer->file));
            fseek(lexer->file, -1, SEEK_CUR);
            if (!isalnum(next))
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
            write_to_lexeme(lexer, cursor);
            lexer->state = READING;
            break;
        case '[':
            return OPEN_ADDRESS;
        case ']':
            return CLOSE_ADDRESS;
        case ',':
            return COMMA;
        case ':':
            return COLON;
        default:
            if (isalnum(cursor))
            {
                write_to_lexeme(lexer, cursor);
                lexer->state = READING;
                break;
            }
            printf("Unexpected character %c.\n", cursor);
            exit(EXIT_FAILURE);
        }
        break;
    case READING:
        if (lexer->lexeme_size == 2 && cursor == 'H')
        {
            write_to_lexeme(lexer, cursor);

            char next = getc(lexer->file);
            fseek(lexer->file, -1, SEEK_CUR);

            if (isspace(next) || next == ',')
            {
                return CONSTANT;
            }
            break;
        }

        if (isalnum(cursor))
        {
            write_to_lexeme(lexer, cursor);
            break;
        }

        if (isspace(cursor) || cursor == EOF)
        {
            if (cursor == '\n')
                fseek(lexer->file, -1, SEEK_CUR);

            if (hm_fetch(lexer->symbols, lexer->lexeme_buffer) != NULL)
                return MNEMONIC;
            return LABEL;
        }

        if (cursor == ':')
        {
            fseek(lexer->file, -1, SEEK_CUR);
            return LABEL;
        }

        printf("Unexpected character %c.\n", cursor);
        exit(EXIT_FAILURE);
    }

    return 0;
}