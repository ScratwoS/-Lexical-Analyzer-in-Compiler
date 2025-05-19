#pragma once

#include <stdbool.h>

#define MAX_SYMBOLS 256
#define MAX_NAME_LEN 32

typedef enum
{
    TYPE_INT
} VarType;

void initSymbolTable(void);
void declareVariable(const char *name, VarType type);
void checkVariable(const char *name);
void semanticError(const char *message, const char *name);
void cleanupSymbolTable(void);
