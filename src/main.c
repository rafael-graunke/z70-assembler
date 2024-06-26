#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Missing arguments:\n\tz70asm <source> <target>\n");
        return EXIT_FAILURE;
    }

    char *input = argv[1];
    char *output = argv[2];

    FILE *source;
    if ((source = fopen(input, "r")) == NULL)
    {
        printf("Cannot open input file.\n");
        return EXIT_FAILURE;
    }

    FILE *target;
    if ((target = fopen(output, "wb")) == NULL)
    {
        printf("Cannot open output file.\n");
        return EXIT_FAILURE;
    }

    // if (run(source, target))
    // {
    //     printf("Error assembling code.\n");
    //     return EXIT_FAILURE;
    // }

    Parser *parser = create_parser(source);
    first_pass(parser);

    destroy_parser(parser);
    printf("Binary assembled to \"%s\".\n", output);
    return EXIT_SUCCESS;
}
