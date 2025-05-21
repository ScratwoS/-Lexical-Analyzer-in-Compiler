#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

TokenType Token;
int Num;
char Id[MAX_IDENT_LEN + 1];
char Ch;
FILE *input;

static int lineNumber = 1;

char getCh()
{
    Ch = fgetc(input);
    if (Ch == '\n')
    {
        lineNumber++;
    }
    return Ch;
}

int getLineNumber()
{
    return lineNumber;
}

const char *getTokenName(TokenType token)
{
    static const char *tokenNames[] = {
        "NONE", "IDENT", "NUMBER",
        "BEGIN", "CALL", "CONST", "DO", "ELSE", "END", "FOR", "IF", "ODD",
        "PROCEDURE", "PROGRAM", "THEN", "TO", "VAR", "WHILE",
        "PLUS", "MINUS", "TIMES", "SLASH", "EQU", "NEQ", "LSS", "LEQ",
        "GTR", "GEQ", "LPARENT", "RPARENT", "LBRACK", "RBRACK",
        "PERIOD", "COMMA", "SEMICOLON", "ASSIGN", "PERCENT"};
    return tokenNames[token];
}

TokenType check(char *str)
{
    if (strcmp(str, "begin") == 0)
        return BEGIN;
    if (strcmp(str, "call") == 0)
        return CALL;
    if (strcmp(str, "const") == 0)
        return CONST;
    if (strcmp(str, "do") == 0)
        return DO;
    if (strcmp(str, "else") == 0)
        return ELSE;
    if (strcmp(str, "end") == 0)
        return END;
    if (strcmp(str, "for") == 0)
        return FOR;
    if (strcmp(str, "if") == 0)
        return IF;
    if (strcmp(str, "odd") == 0)
        return ODD;
    if (strcmp(str, "procedure") == 0)
        return PROCEDURE;
    if (strcmp(str, "program") == 0)
        return PROGRAM;
    if (strcmp(str, "then") == 0)
        return THEN;
    if (strcmp(str, "to") == 0)
        return TO;
    if (strcmp(str, "var") == 0)
        return VAR;
    if (strcmp(str, "while") == 0)
        return WHILE;
    return IDENT;
}

TokenType getToken()
{
    while (isspace(Ch))
        getCh();

    if (isalpha(Ch))
    {
        int i = 0;
        do
        {
            if (i < MAX_IDENT_LEN)
                Id[i++] = tolower(Ch);
            getCh();
        } while (isalnum(Ch));
        Id[i] = '\0';
        return (Token = check(Id));
    }
    else if (isdigit(Ch))
    {
        Num = 0;
        do
        {
            Num = Num * 10 + (Ch - '0');
            getCh();
        } while (isdigit(Ch));
        return (Token = NUMBER);
    }
    else
    {
        switch (Ch)
        {
        case '+':
            getCh();
            return (Token = PLUS);
        case '-':
            getCh();
            return (Token = MINUS);
        case '*':
            getCh();
            return (Token = TIMES);
        case '/':
            getCh();
            return (Token = SLASH);
        case '=':
            getCh();
            return (Token = EQU);
        case '<':
            getCh();
            if (Ch == '=')
            {
                getCh();
                return (Token = LEQ);
            }
            else if (Ch == '>')
            {
                getCh();
                return (Token = NEQ);
            }
            else
                return (Token = LSS);
        case '>':
            getCh();
            if (Ch == '=')
            {
                getCh();
                return (Token = GEQ);
            }
            else
                return (Token = GTR);
        case '(':
            getCh();
            return (Token = LPARENT);
        case ')':
            getCh();
            return (Token = RPARENT);
        case '[':
            getCh();
            return (Token = LBRACK);
        case ']':
            getCh();
            return (Token = RBRACK);
        case '.':
            getCh();
            return (Token = PERIOD);
        case ',':
            getCh();
            return (Token = COMMA);
        case ';':
            getCh();
            return (Token = SEMICOLON);
        case ':':
            getCh();
            if (Ch == '=')
            {
                getCh();
                return (Token = ASSIGN);
            }
            else
            {
                printf("Error at line %d: Expected '=' after ':'\n", getLineNumber());
                break;
            }
        case '%':
            getCh();
            return (Token = PERCENT);
        case EOF:
            return (Token = NONE);
        default:
            printf("Unknown character at line %d: %c (ASCII: %d)\n", getLineNumber(), Ch, Ch);
            getCh();
            return getToken();
        }
    }

    return (Token = NONE);
}

void printToken()
{
    printf("%s", getTokenName(Token));
    if (Token == IDENT)
    {
        printf("(%s)", Id);
    }
    else if (Token == NUMBER)
    {
        printf("(%d)", Num);
    }
    printf("\n");
}

void initLexer(const char *File)
{
    input = fopen(File, "r");
    if (!input)
    {
        perror("Error opening file");
        exit(1);
    }
    Ch = ' ';
    lineNumber = 1; // reset line count khi mở file mới
}

void closeLexer()
{
    if (input)
        fclose(input);
}
