#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdint.h>
#include <stdbool.h>
#include "hashmap.h"
#include "lexer.h"

#define MAX_LABELS_AMOUNT 100

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