#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

static Symbol symTable[MAX_SYMBOLS];
static int symCount = 0;

void initSymbolTable(void)
{
    symCount = 0;
}

void cleanupSymbolTable(void)
{
}

void semanticError(const char *message, const char *name)
{
    if (name && name[0] != '\0')
        fprintf(stderr, "Loi ngu nghia o dong %d: %s '%s'\n", getLineNumber(), message, name);
    else
        fprintf(stderr, "Loi ngu nghia o dong %d: %s\n", getLineNumber(), message);
    exit(1);
}

void declareVariableWithSize(const char *name, VarType type, int size)
{
    for (int i = 0; i < symCount; i++)
    {
        if (strcmp(symTable[i].name, name) == 0)
        {
            semanticError("Bien da duoc khai bao", name);
        }
    }
    if (symCount >= MAX_SYMBOLS)
    {
        semanticError("Bang ky hieu day", "");
    }
    strncpy(symTable[symCount].name, name, MAX_NAME_LEN);
    symTable[symCount].type = type;
    symTable[symCount].size = size;
    symTable[symCount].isValueKnown = 0;
    symCount++;
}

void declareConstant(const char *name, int value)
{
    for (int i = 0; i < symCount; i++)
    {
        if (strcmp(symTable[i].name, name) == 0)
        {
            semanticError("Bien da duoc khai bao", name);
        }
    }
    if (symCount >= MAX_SYMBOLS)
    {
        semanticError("Bang ky hieu day", "");
    }
    strncpy(symTable[symCount].name, name, MAX_NAME_LEN);
    symTable[symCount].type = TYPE_CONST;
    symTable[symCount].size = 0;
    symTable[symCount].constValue = value;
    symTable[symCount].curValue = value;
    symTable[symCount].isValueKnown = 1;
    symCount++;
}

void checkVariable(const char *name)
{
    for (int i = 0; i < symCount; i++)
    {
        if (strcmp(symTable[i].name, name) == 0)
        {
            return;
        }
    }
    semanticError("Bien chua duoc khai bao", name);
}

void checkArrayIndex(const char *name, int index)
{
    for (int i = 0; i < symCount; i++)
    {
        if (strcmp(symTable[i].name, name) == 0)
        {
            if (symTable[i].size == 0)
            {
                semanticError("Bien khong phai mang", name);
            }
            if (index < 0 || index >= symTable[i].size)
            {
                semanticError("Chi so mang ngoai pham vi", name);
            }
            return;
        }
    }
    semanticError("Bien chua duoc khai bao", name);
}

void checkDivideByZero(int divisor)
{
    if (divisor == 0)
    {
        semanticError("Chia cho 0", "");
    }
}

void checkArrayIndexUnknownIndex(const char *name)
{
    for (int i = 0; i < symCount; i++)
    {
        if (strcmp(symTable[i].name, name) == 0)
        {
            if (symTable[i].size == 0)
            {
                semanticError("Bien khong phai mang", name);
            }
            return;
        }
    }
    semanticError("Bien chua duoc khai bao", name);
}

int getVariableValue(const char *name, int *isKnown)
{
    for (int i = 0; i < symCount; i++)
    {
        if (strcmp(symTable[i].name, name) == 0)
        {
            if (symTable[i].type == TYPE_CONST)
            {
                *isKnown = 1;
                return symTable[i].constValue;
            }
            if (symTable[i].isValueKnown)
            {
                *isKnown = 1;
                return symTable[i].curValue;
            }
            break;
        }
    }
    *isKnown = 0;
    return 0;
}

void setVariableValue(const char *name, int value)
{
    for (int i = 0; i < symCount; i++)
    {
        if (strcmp(symTable[i].name, name) == 0)
        {
            symTable[i].curValue = value;
            symTable[i].isValueKnown = 1;
            return;
        }
    }
    semanticError("Bien chua duoc khai bao khi gan gia tri", name);
}
