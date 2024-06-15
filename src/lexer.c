#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hashmap.h"
#include "lexer.h"

HashMap *init_symbols(void)
{
    HashMap *hashmap = create_hashmap(18);
    hashmap_insert(hashmap, "ADD", 0x00);
    hashmap_insert(hashmap, "SUB", 0x10);
    hashmap_insert(hashmap, "CMP", 0x20);
    hashmap_insert(hashmap, "INC", 0x30);
    hashmap_insert(hashmap, "DEC", 0x40);
    hashmap_insert(hashmap, "AND", 0x50);
    hashmap_insert(hashmap, "OR", 0x60);
    hashmap_insert(hashmap, "NOT", 0x70);
    hashmap_insert(hashmap, "SHR", 0x80);
    hashmap_insert(hashmap, "SHL", 0x90);
    hashmap_insert(hashmap, "JMP", 0xA0);
    hashmap_insert(hashmap, "JZ", 0xA1);
    hashmap_insert(hashmap, "JS", 0xA2);
    hashmap_insert(hashmap, "JC", 0xA3);
    hashmap_insert(hashmap, "JO", 0xA4);
    hashmap_insert(hashmap, "JP", 0xA5);
    hashmap_insert(hashmap, "MOV", 0xB0);
    hashmap_insert(hashmap, "NOP", 0xFF);

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
    destroy_hashmap(lexer->symbols);
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
    case INSTRUCTION:
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

            if (hashmap_fetch(lexer->symbols, lexer->lexeme_buffer) != -1)
                return INSTRUCTION;
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