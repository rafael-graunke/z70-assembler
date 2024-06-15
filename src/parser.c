#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

typedef struct hm_value
{
    int value;
} Address;

Address *create_addr(int value)
{
    Address *addr = (Address *)malloc(sizeof(Address));
    addr->value = value;

    return addr;
}

HashMap *init_addressing(void)
{
    HashMap *hashmap = hm_create(18);

    // Two operand
    hm_insert(hashmap, "AB", create_addr(0x0));
    hm_insert(hashmap, "BA", create_addr(0x1));
    hm_insert(hashmap, "AI", create_addr(0x2));
    hm_insert(hashmap, "IA", create_addr(0x3));
    hm_insert(hashmap, "AMI", create_addr(0x4));
    hm_insert(hashmap, "MIA", create_addr(0x5));
    hm_insert(hashmap, "AC", create_addr(0x6));
    hm_insert(hashmap, "BC", create_addr(0x7));
    hm_insert(hashmap, "IC", create_addr(0x8));
    hm_insert(hashmap, "MIC", create_addr(0x9));
    hm_insert(hashmap, "AMC", create_addr(0xA));
    hm_insert(hashmap, "BMC", create_addr(0xB));
    hm_insert(hashmap, "MCA", create_addr(0xC));
    hm_insert(hashmap, "MCB", create_addr(0xD));

    // Single operand
    hm_insert(hashmap, "A", create_addr(0x0));
    hm_insert(hashmap, "B", create_addr(0x1));
    hm_insert(hashmap, "I", create_addr(0x2));
    hm_insert(hashmap, "MI", create_addr(0x4));

    return hashmap;
}

Parser *create_parser(FILE *file)
{
    Parser *parser = (Parser *)malloc(sizeof(Parser));
    parser->lexer = create_lexer(file);
    parser->labels = hm_create(MAX_LABELS_AMOUNT);
    parser->addressing = init_addressing();

    return parser;
}

void destroy_parser(Parser *parser)
{
    destroy_lexer(parser->lexer);
    hm_destroy(parser->labels);
    hm_destroy(parser->addressing);
    free(parser);
}

void first_pass(Parser *parser)
{
    int address = 0;
    Token token;
    while ((token = advance(parser->lexer)).type != _EOF)
    {
        if (token.type == MNEMONIC || token.type == CONSTANT)
        {
            address++;
            continue;
        }

        if (token.type == LABEL && (token = advance(parser->lexer)).type == COLON)
            hm_insert(parser->labels, token.lexval, create_addr(address));
    }

    fseek(parser->lexer->file, 0, SEEK_SET);
}

void second_pass(Parser *parser)
{

}
