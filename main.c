#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "scanner.h"
#include "semantic.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input_filename>\n", argv[0]);
        return 1;
    }
    const char *filename = argv[1];

    initLexer(filename);
    initSymbolTable();
    Program();
    cleanupSymbolTable();
    closeLexer();

    fprintf(stderr, "Phan tich cu phap va ngu nghia thanh cong!\n");
    return 0;
}
