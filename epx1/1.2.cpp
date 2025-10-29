#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define OP_NUM 9

typedef enum
{
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_POW,
    OP_FAC,
    OP_L,
    OP_R,
    OP_END
} OPID;

const char level[OP_NUM][OP_NUM] = {
    {'>', '>', '<', '<', '<', '<', '<', '>', '>'},
    {'>', '>', '<', '<', '<', '<', '<', '>', '>'},
    {'>', '>', '>', '>', '<', '<', '<', '>', '>'},
    {'>', '>', '>', '>', '<', '<', '<', '>', '>'},
    {'>', '>', '>', '>', '>', '<', '<', '>', '>'},
    {'>', '>', '>', '>', '>', '>', ' ', '>', '>'},
    {'<', '<', '<', '<', '<', '<', '<', '=', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>'},
    {'<', '<', '<', '<', '<', '<', '<', ' ', '='}};

double numStack[100];
char opStack[100];
int nTop = -1, oTop = -1;

void pushN(double x) { numStack[++nTop] = x; }
double popN() { return numStack[nTop--]; }

void pushO(char c) { opStack[++oTop] = c; }
char popO() { return opStack[oTop--]; }

OPID id(char c)
{
    switch (c)
    {
    case '+':
        return OP_ADD;
    case '-':
        return OP_SUB;
    case '*':
        return OP_MUL;
    case '/':
        return OP_DIV;
    case '^':
        return OP_POW;
    case '!':
        return OP_FAC;
    case '(':
        return OP_L;
    case ')':
        return OP_R;
    case '\0':
        return OP_END;
    }
    return OP_END;
}

void doCalc()
{
    char op = popO();
    double a, b;
    switch (op)
    {
    case '+':
        b = popN();
        a = popN();
        pushN(a + b);
        break;
    case '-':
        b = popN();
        a = popN();
        pushN(a - b);
        break;
    case '*':
        b = popN();
        a = popN();
        pushN(a * b);
        break;
    case '/':
        b = popN();
        a = popN();
        if (b == 0)
        {
            printf("错误：除0\n");
            exit(0);
        }
        pushN(a / b);
        break;
    case '^':
        b = popN();
        a = popN();
        pushN(pow(a, b));
        break;
    case '!':
        a = popN();
        {
            double m = 1;
            for (int i = 1; i <= (int)a; i++)
                m *= i;
            pushN(m);
        }
        break;
    }
}

double calc(char *s)
{
    nTop = oTop = -1;
    pushO('\0');

    for (int i = 0;;)
    {
        if (isdigit(s[i]))
        {
            double v = 0;
            while (isdigit(s[i]))
            {
                v = v * 10 + (s[i] - '0');
                i++;
            }
            pushN(v);
        }
        else
        {
            OPID cur = id(s[i]);
            OPID top = id(opStack[oTop]);
            char rel = level[top][cur];

            if (rel == '<')
            {
                pushO(s[i]);
                i++;
            }
            else if (rel == '=')
            {
                popO();
                i++;
            }
            else if (rel == '>')
            {
                doCalc();
            }
            else
            {
                printf("表达式无效：%s\n", s);
                exit(0);
            }
        }
        if (s[i] == '\0' && opStack[oTop] == '\0')
            break;
    }
    return popN();
}

int main()
{
    char *tests[] = {
        "1+2*3",
        "(2+3)*4",
        "10/(5-3)",
        "2^3^2",
        "5!",
        "3+5*2-8/4",
        "7+(6*5^2+3)",
        NULL};

    for (int t = 0; tests[t] != NULL; t++)
    {
        double r = calc(tests[t]);
        printf("表达式: %-12s  结果: %.2f\n", tests[t], r);
    }
    return 0;
}
