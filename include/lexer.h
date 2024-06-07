#ifndef _LEXER_H_
#define _LEXER_H_

#include <stdio.h>

/*
    States of the Lexer State Machine.

    More context can by found on the `docs` subdirectory
    inside the project folder.
*/
#define START 0
#define READING 1

#define MAX_LEXEME_SIZE 256

enum token {
    LABEL = 1,
    INSTRUCTION = 2,
    A_REGISTER = 3,
    B_REGISTER = 4,
    I_REGISTER = 5,
    CONSTANT = 6,
    OPEN_ADDRESS = 7,
    CLOSE_ADDRESS = 8,
    COMMA = 9,
    _EOF = 10
};

enum token advance(FILE *f);
int step(FILE *f);

#endif