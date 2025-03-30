#include "scanner.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input-file>\n", argv[0]);
        return 1;
    }

    initLexer(argv[1]);

    while (1) {
        getToken();
        if (Token == NONE) break;
        printToken();
    }

    closeLexer();
    return 0;
}

