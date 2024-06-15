#ifndef _LEXER_H_
#define _LEXER_H_

#include <stdio.h>
#include "hashmap.h"

/*
    States of the Lexer State Machine.

    More context can by found on the `docs` subdirectory
    inside the project folder.
*/
#define START 0
#define READING 1

#define MAX_LEXEME_SIZE 256

typedef enum toktype
{
    LABEL = 1,
    INSTRUCTION = 2,
    A_REGISTER = 3,
    B_REGISTER = 4,
    I_REGISTER = 5,
    CONSTANT = 6,
    OPEN_ADDRESS = 7,
    CLOSE_ADDRESS = 8,
    COMMA = 9,
    EOL = 10,
    COLON = 11,
    _EOF = 99
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

Lexer *create_lexer(FILE *f);
void destroy_lexer(Lexer *lexer);
Token advance(Lexer *lexer);
int step(Lexer *lexer);

#endif