#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "scanner.h"
#include "semantic.h"

// prototypes để compiler biết trước khi sử dụng
static int evalFactor(int *isKnown);
static int evalTerm(int *isKnown);
static int evalExpression(int *isKnown);

void error(const char *message)
{
    fprintf(stderr, "Loi phan tich cu phap o dong %d: %s. Token hien tai: %s",
            getLineNumber(), message, getTokenName(Token));
    if (Token == IDENT)
        fprintf(stderr, " (%s)", Id);
    if (Token == NUMBER)
        fprintf(stderr, " (%d)", Num);
    fprintf(stderr, "\n");
    exit(1);
}

void match(TokenType expected)
{
    if (Token == expected)
        getToken();
    else
    {
        char error_msg[150];
        snprintf(error_msg, sizeof(error_msg),
                 "Ky vong token '%s', nhung nhan duoc '%s'",
                 getTokenName(expected), getTokenName(Token));
        error(error_msg);
    }
}

// Đánh giá giá trị factor
static int evalFactor(int *isKnown)
{
    int value = 0;
    if (Token == IDENT)
    {
        checkVariable(Id);
        char varName[MAX_IDENT_LEN + 1];
        strcpy(varName, Id);

        int hasIndex = 0; // Kiểm tra truy cập mảng hay biến thường
        getToken();

        if (Token == LBRACK)
        {
            hasIndex = 1;
            getToken();

            if (Token == NUMBER)
            {
                int idx = Num;
                getToken();
                match(RBRACK);
                checkArrayIndex(varName, idx);
            }
            else if (Token == IDENT)
            {
                checkVariable(Id);
                checkIndexVariable(Id);
                getToken();
                match(RBRACK);
                checkArrayIndexUnknownIndex(varName);
            }
            else
            {
                error("Chi so mang phai la so nguyen hoac bien");
            }
        }

        // Kiểm tra kiểu truy cập đúng: biến mảng phải có chỉ số, biến thường không được có chỉ số
        checkArrayUsage(varName, hasIndex);

        int known;
        value = getVariableValue(varName, &known);
        *isKnown = known;
        return value;
    }
    else if (Token == NUMBER)
    {
        value = Num;
        *isKnown = 1;
        getToken();
        return value;
    }
    else if (Token == LPARENT)
    {
        getToken();
        value = evalExpression(isKnown);
        match(RPARENT);
        return value;
    }
    else
    {
        error("Loi factor: Ky vong dinh danh, so, hoac '('");
        return 0;
    }
}

// Đánh giá term
static int evalTerm(int *isKnown)
{
    int value = evalFactor(isKnown);
    while (Token == TIMES || Token == SLASH)
    {
        TokenType op = Token;
        getToken();
        int rhsVal = evalFactor(isKnown);
        int rhsKnown = *isKnown;

        if (op == SLASH)
        {
            if (rhsKnown && rhsVal == 0)
                semanticError("Chia cho 0", "");
            if (*isKnown && rhsKnown)
                value /= rhsVal;
            else
                *isKnown = 0;
        }
        else // TIMES
        {
            if (*isKnown && rhsKnown)
                value *= rhsVal;
            else
                *isKnown = 0;
        }
    }
    return value;
}

// Đánh giá expression
static int evalExpression(int *isKnown)
{
    int value = 0;
    *isKnown = 1;
    int sign = 1;

    if (Token == PLUS)
        getToken();
    else if (Token == MINUS)
    {
        sign = -1;
        getToken();
    }

    value = evalTerm(isKnown);
    if (*isKnown)
        value *= sign;

    while (Token == PLUS || Token == MINUS)
    {
        TokenType op = Token;
        getToken();
        int rhsVal = evalTerm(isKnown);
        int rhsKnown = *isKnown;

        if (*isKnown && rhsKnown)
        {
            if (op == PLUS)
                value += rhsVal;
            else if (op == MINUS)
                value -= rhsVal;
        }
        else
        {
            *isKnown = 0;
        }
    }
    return value;
}

// Giữ nguyên để tương thích
void expression()
{
    int d;
    (void)evalExpression(&d);
}
void term()
{
    int d;
    (void)evalTerm(&d);
}
void factor()
{
    int d;
    (void)evalFactor(&d);
}

void condition()
{
    expression();
    if (Token == EQU || Token == NEQ || Token == LSS ||
        Token == LEQ || Token == GTR || Token == GEQ)
    {
        getToken();
        expression();
    }
    else
    {
        error("Dieu kien loi: Ky vong toan tu quan he (=, <>, <, <=, >, >=)");
    }
}

void statement()
{
    switch (Token)
    {
    case IDENT:
    {
        char varName[MAX_IDENT_LEN + 1];
        strcpy(varName, Id);
        int hasIndex = 0;

        getToken();

        if (Token == LBRACK)
        {
            hasIndex = 1;
            getToken();

            if (Token == NUMBER)
            {
                int idx = Num;
                getToken();
                match(RBRACK);
                checkArrayIndex(varName, idx);
            }
            else if (Token == IDENT)
            {
                checkVariable(Id);
                checkIndexVariable(Id);
                getToken();
                match(RBRACK);
                checkArrayIndexUnknownIndex(varName);
            }
            else
            {
                error("Chi so mang phai la so nguyen hoac bien");
            }
        }

        checkArrayUsage(varName, hasIndex); // Kiểm tra biến/mảng và kiểu truy cập

        match(ASSIGN);
        int known, val = evalExpression(&known);
        if (known)
            setVariableValue(varName, val);
        break;
    }

    case CALL:
    {
        getToken(); // ăn CALL
        if (Token != IDENT)
            error("CALL phai theo sau IDENT");
        char procName[MAX_IDENT_LEN + 1];
        strcpy(procName, Id);
        checkProcedure(procName);
        getToken();
        if (Token == LPARENT)
        {
            getToken();
            if (Token != RPARENT)
            {
                expression();
                while (Token == COMMA)
                {
                    getToken();
                    expression();
                }
            }
            match(RPARENT);
        }
        break;
    }

    case BEGIN:
        getToken();
        statement();
        while (Token == SEMICOLON)
        {
            getToken();
            if (Token == END)
                break;
            statement();
        }
        match(END);
        break;

    case IF:
        getToken();
        condition();
        match(THEN);
        statement();
        while (Token == SEMICOLON)
        {
            getToken();
            statement();
        }
        if (Token == ELSE)
        {
            getToken();
            statement();
        }
        break;

    case WHILE:
        getToken();
        condition();
        match(DO);
        statement();
        break;

    case FOR:
        getToken();
        match(IDENT);
        match(ASSIGN);
        expression();
        match(TO);
        expression();
        match(DO);
        statement();
        break;

    default:
        error("Bat dau cau lenh khong hop le");
    }
}

void block()
{
    if (Token == CONST)
    {
        getToken();
        do
        {
            match(IDENT);
            char constName[MAX_IDENT_LEN + 1];
            strcpy(constName, Id);
            match(EQU);
            if (Token != NUMBER)
                error("Gia tri hang so phai la so nguyen");
            declareConstant(constName, Num);
            getToken();
        } while (Token == COMMA && (getToken(), 1));
        match(SEMICOLON);
    }

    if (Token == VAR)
    {
        getToken();
        do
        {
            match(IDENT);
            char varName[MAX_IDENT_LEN + 1];
            strcpy(varName, Id);
            int arrSize = 0;
            if (Token == LBRACK)
            {
                getToken();
                if (Token != NUMBER)
                    error("Kich thuoc mang phai la so nguyen duong");
                arrSize = Num;
                getToken();
                match(RBRACK);
            }
            declareVariableWithSize(varName, TYPE_INT, arrSize);
        } while (Token == COMMA && (getToken(), 1));
        match(SEMICOLON);
    }

    while (Token == PROCEDURE)
    {
        getToken();
        if (Token != IDENT)
            error("PROCEDURE phai theo sau IDENT");
        char procName[MAX_IDENT_LEN + 1];
        strcpy(procName, Id);
        declareProcedure(procName);
        getToken();
        match(SEMICOLON);
        block();
        match(SEMICOLON);
    }

    statement();
}

void Program()
{
    getToken();
    match(PROGRAM);
    match(IDENT);
    match(SEMICOLON);
    block();
    match(PERIOD);
    if (Token != NONE)
        error("Ky tu ngoai le sau dau '.'");
}
