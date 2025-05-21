#define MAX_SYMBOLS 100
#define MAX_NAME_LEN 32

typedef enum
{
    TYPE_CONST,
    TYPE_INT
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
void declareConstant(const char *name, int value);
void checkVariable(const char *name);
void checkArrayIndex(const char *name, int index);
void checkDivideByZero(int divisor);
void checkArrayIndexUnknownIndex(const char *name);
int getVariableValue(const char *name, int *isKnown);
void setVariableValue(const char *name, int value);
