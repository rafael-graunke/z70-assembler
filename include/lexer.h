#ifndef _LEXER_H_
#define _LEXER_H_

#include <stdio.h>
#include <stdint.h>
#include "hashmap.h"

/*
    States of the Lexer State Machine.

    More context can by found on the `docs` subdirectory
    inside the project folder.
*/
#define START 0
#define READING 1

#define MAX_LEXEME_SIZE 256

typedef struct mnemonic
{
    uint8_t value;
    int op_count;
} Mnemonic;

typedef enum toktype
{
    LABEL = 1,
    MNEMONIC = 2,
    REGISTER = 3,
    CONSTANT = 4,
    OPEN_ADDRESS = 5,
    CLOSE_ADDRESS = 6,
    COMMA = 7,
    EOL = 8,
    COLON = 9,
    _EOF = 10
} TokenType;

typedef struct
{
    char lexval[MAX_LEXEME_SIZE];
    TokenType type;
} Token;

typedef struct
{
    char lexeme_buffer[MAX_LEXEME_SIZE];
    int state;
    int lexeme_size;
    FILE *file;
    HashMap *symbols;
} Lexer;

Lexer *lx_create(FILE *f);
void lx_destroy(Lexer *lexer);
Mnemonic *mn_fetch(Lexer *lexer, char *key);
Token lx_advance(Lexer *lexer);

#endif