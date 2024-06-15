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

    bin -> {line} EOL
    line -> (instruction | label_declaration) EOL
    instruction -> [label_declaration] MNEMONIC [operands]
    label_declaration -> LABEL COLON
    operands -> (operands COMMA) | operand
    operand -> CONSTANT | register | memory_addr
    register -> A_REGISTER | B_REGISTER | I_REGISTER
    memory_addr -> OPEN_ADDRESS (I_REGISTER | CONSTANT) CLOSE_ADDRESS
*/

typedef struct
{
    HashMap *labels;
    HashMap *addressing;
    Lexer *lexer;
} Parser;

Parser *create_parser(FILE *file);
void destroy_parser(Parser *parser);
void first_pass(Parser *parser); // TODO: remove this

#endif