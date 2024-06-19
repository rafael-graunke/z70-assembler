#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdint.h>
#include <stdbool.h>
#include "hashmap.h"
#include "lexer.h"

#define MAX_LABELS_AMOUNT 100

/*
    TODO: check this again
    This is a brief grammar definition for the Z70 Assembly

    line -> (LABEL COLON | MNEMONIC operators) EOL
    operators -> memory_address | CONSTANT | REGISTER | empty
    memory_address -> OPEN_ADDRESS (REGISTER | CONSTANT) CLOSE_ADDRESS
*/

typedef struct
{
    HashMap *labels;
    HashMap *addressing;
    Lexer *lexer;
    Token lookahead;
    FILE *target;
} Parser;

Parser *create_parser(FILE *source, FILE *target);
void destroy_parser(Parser *parser);
void first_pass(Parser *parser);  // TODO: remove this
void second_pass(Parser *parser); // TODO: remove this

#endif