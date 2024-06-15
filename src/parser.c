#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

HashMap *init_addressing(void)
{
    HashMap *hashmap = create_hashmap(18);

    // Two operand
    hashmap_insert(hashmap, "AB", 0x0);
    hashmap_insert(hashmap, "BA", 0x1);
    hashmap_insert(hashmap, "AI", 0x2);
    hashmap_insert(hashmap, "IA", 0x3);
    hashmap_insert(hashmap, "AMI", 0x4);
    hashmap_insert(hashmap, "MIA", 0x5);
    hashmap_insert(hashmap, "AC", 0x6);
    hashmap_insert(hashmap, "BC", 0x7);
    hashmap_insert(hashmap, "IC", 0x8);
    hashmap_insert(hashmap, "MIC", 0x9);
    hashmap_insert(hashmap, "AMC", 0xA);
    hashmap_insert(hashmap, "BMC", 0xB);
    hashmap_insert(hashmap, "MCA", 0xC);
    hashmap_insert(hashmap, "MCB", 0xD);

    // Single operand
    hashmap_insert(hashmap, "A", 0x0);
    hashmap_insert(hashmap, "B", 0x1);
    hashmap_insert(hashmap, "I", 0x2);
    hashmap_insert(hashmap, "MI", 0x4);

    return hashmap;
}

Parser *create_parser(FILE *file)
{
    Parser *parser = (Parser *)malloc(sizeof(Parser));
    parser->lexer = create_lexer(file);
    parser->labels = create_hashmap(MAX_LABELS_AMOUNT);
    parser->addressing = init_addressing();

    return parser;
}

void destroy_parser(Parser *parser)
{
    destroy_lexer(parser->lexer);
    destroy_hashmap(parser->labels);
    destroy_hashmap(parser->addressing);
    free(parser);
}

void first_pass(Parser *parser)
{
    int address = 0;
    Token token;
    while ((token = advance(parser->lexer)).type != _EOF)
    {
        if (token.type == INSTRUCTION || token.type == CONSTANT)
        {
            address++;
            continue;
        }

        if (token.type == LABEL && (token = advance(parser->lexer)).type == COLON)
            hashmap_insert(parser->labels, token.lexval, address);
    }

    fseek(parser->lexer->file, 0, SEEK_SET);
}

void second_pass(Parser *parser)
{
}
