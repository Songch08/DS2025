#include "Stack.h"
#include <iostream>
#include <cctype>
#include <cmath>
#include <cstdlib>
using namespace std;

template <typename T>
void readNumber(char*& p, Stack<T>& stk)
{
    stk.push((T)(*p - '0'));           
    while (isdigit(*(++p)))            
        stk.top() = 10 * stk.top() + (*p - '0');
    if ('.' != *p) return;           

    float fraction = 1;
    while (isdigit(*(++p))) {          
        fraction /= 10;
        stk.top() += (*p - '0') * fraction;
    }
}
void convert(Stack<int>& S, _int64 n,int base) {
    static char digits[] 
    = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    while (n>0){
        int remainder = (int)(n%base);S.push(digits[remainder]);
        n /= base;
    }
}
void trim(const char exp[],int& lo,int& hi){
    while ((lo<=hi)&&(exp[lo]!='(')&&(exp[lo]!=')'))lo++;
    while ((lo<=hi)&&(exp[hi]!='(')&&(exp[hi]!=')'))hi--;
}

int divide(const char exp[],int lo,int hi){
    int mi = lo;int crc=1;
    while ((0<crc)&&(++mi<=hi))
       {if(exp[mi]==')')crc--;if(exp[mi]=='(')crc++;}
    return mi;
}
bool paren(const char exp[],int lo,int hi){
    Stack<char> S;
    for (int i=lo;i<=hi;i++)
      switch (exp[i]){
        case '(':case '[':case '{':S.push(exp[i]);break;
        case ')':if ((S.empty())||('('!=S.pop()))return false;break;
        case ']':if ((S.empty())||('['!=S.pop()))return false;break;
        case '}':if ((S.empty())||('{'!=S.pop()))return false;break;
        default:break;
    }
    return S.empty();
}
#define N_OPTR 9
enum Operator {ADD,SUB,MUL,DIV,POW,FAC,L_P,R_P,EOE};

const char pri[N_OPTR][N_OPTR]={
/*                 |--------------------| */
/*  |       +   -   *   /   ^   !   (   )   \0 */
/* -- + */ '>','>','<','<','<','<','<','>','>',
/* -- - */ '>','>','<','<','<','<','<','>','>',
/* -- * */ '>','>','>','>','<','<','<','>','>',
/* -- / */ '>','>','>','>','<','<','<','>','>',
/* -- ^ */ '>','>','>','>','>','<','<','>','>',
/* -- ! */ '>','>','>','>','>','>',' ','>','>',
/* -- ( */ '<','<','<','<','<','<','<','=',' ',
/* -- ) */ ' ',' ',' ',' ',' ',' ',' ',' ',' ',
/* -- \0 */ '<','<','<','<','<','<','<',' ','='
};
Operator optr2rank(char op)
{
    switch (op) {
    case '+': return ADD;
    case '-': return SUB;
    case '*': return MUL;
    case '/': return DIV;
    case '^': return POW;
    case '!': return FAC;
    case '(': return L_P;
    case ')': return R_P;
    case '\0': return EOE;
    default:  exit(-1);
    }
}
char orderBetween(char op1, char op2)
{
    return pri[optr2rank(op1)][optr2rank(op2)];
}

float calcu(char op, float n)
{
    switch (op) {
    case '!': {
        float r = 1;
        for (int i = 1; i <= (int)n; i++) r *= i;
        return r;
    }
    default: exit(-1);
    }
}

float calcu(float a, char op, float b)
{
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return a / b;
    case '^': return pow(a, b);
    default: exit(-1);
    }
}

void append(char*& RPN, char op)
{
    *RPN++ = op;
    *RPN++ = ' ';
}
float evaluate ( char*S,char*& RPN){
    Stack<float> opnd;Stack<char> optr;
    optr.push('\0');
    while(!optr.empty()){
        if (isdigit(*S)){
            readNumber(S,opnd);append(RPN,opnd.top());
        }else
          switch( orderBetween(optr.top(),*S)){
            case '<':
                optr.push(*S);S++;
                break;
            case '=':
                optr.pop();S++;
                break;
            case '>':{
                char op = optr.pop();append(RPN,op);
                if ('!'==op){
                    float p0pnd = opnd.pop();
                    opnd.push(calcu(op,p0pnd));
                }else{
                    float p0pnd2= opnd.pop(),p0pnd1 = opnd.pop();
                    opnd.push(calcu(p0pnd1,op,p0pnd2));
                }
                break;
            }
            default: exit(-1);
        }
    }
    return opnd.top();
}