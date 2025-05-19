#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char symNames[MAX_SYMBOLS][MAX_NAME_LEN];
static VarType symTypes[MAX_SYMBOLS];
static int symCount = 0;

void initSymbolTable(void)
{
    symCount = 0;
}

void semanticError(const char *message, const char *name)
{
    fprintf(stderr, "Loi ngu nghia: %s '%s'\\n", message, name);
    exit(1);
}

void declareVariable(const char *name, VarType type)
{
    for (int i = 0; i < symCount; i++)
    {
        if (strcmp(symNames[i], name) == 0)
        {
            semanticError("Bien da duoc khai bao", name);
        }
    }
    if (symCount >= MAX_SYMBOLS)
    {
        semanticError("Bang ky hieu day", name);
    }
    strncpy(symNames[symCount], name, MAX_NAME_LEN);
    symTypes[symCount++] = type;
}

void checkVariable(const char *name)
{
    for (int i = 0; i < symCount; i++)
    {
        if (strcmp(symNames[i], name) == 0)
        {
            return;
        }
    }
    semanticError("Bien chua duoc khai bao", name);
}

void cleanupSymbolTable(void)
{
}
