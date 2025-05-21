#pragma once

#define MAX_SYMBOLS 100
#define MAX_NAME_LEN 32

typedef enum
{
    TYPE_CONST,
    TYPE_INT,
    TYPE_PROC
} VarType;

typedef struct
{
    char name[MAX_NAME_LEN];
    VarType type;
    int size;
    int constValue;
    int isValueKnown;
    int curValue;
} Symbol;

void initSymbolTable(void);
void cleanupSymbolTable(void);
void semanticError(const char *message, const char *name);
void declareVariableWithSize(const char *name, VarType type, int size);
void declareArray(const char *name, VarType type, int size);
void declareConstant(const char *name, int value);
void declareProcedure(const char *name);
void checkVariable(const char *name);
void checkProcedure(const char *name);
void checkArrayIndex(const char *name, int index);
void checkArrayIndexUnknownIndex(const char *name);
void checkDivideByZero(int divisor);
void setVariableValue(const char *name, int value);
void checkArrayUsage(const char *name, int hasIndex);
int getVariableValue(const char *name, int *isKnown);