#include "Vector.h"      
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <iostream>

template<typename T> class Stack : public Vector<T> {
public:
    void push(T const& e) { insert(size(), e);}
    T pop() {return remove(size()-1);}
    T& top(){return (*this)[size()-1];}
};
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
typename enum {ADD,SUB,MUL,DIV,POW,FAC,L_P,R_P,EOE}Operator;\

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
                    float p0pnd = opnd.pop(),p0pnd1 = opnd.pop();
                    opnd.push(calcu(p0pnd1,op,p0pnd2));
                }
                break;
            }
            default: exit(-1);
        }
    }
    return opnd.top();
}
struct Queen{
    int x,y;
    Queen(int xx=0,int yy=0):x(xx),y(yy){};
    bool operator==(Queen const& q) const{
        return (x==q.x)
                ||(y==q.y)
                ||(x+y==q.x+q.y)
                ||(x-y==q.x-q.y);
    }
    bool operator!=(Queen const& q) const {return !(*this==q);}
}
void placeQueens( int N){
    Stack<Queen> solu;
    Queen q(0,0);
    do{
        if (N<=solu.size()||N<=q.){
            q=solu.pop();q.y++;
        }else{
            while((q.y<N)&&(0<=solu.find(q)))
            {q.y++;nCheck++;}
            if(N>=q.y){
                solu.push(q);
                if(N<=solu.size())nSolu++;
                q.x++;q.y=0;
            }
        }
    }while ((0<q.x)||(q.y<N));
}
typedef enum {AVAILABLE,ROUTE,BACKTRACKED,WALL }Status;

typedef enum {UNKNOWN,EAST,SOUTH,WEST,NORTH,NO_WAY}ESWN

inline ESWN nextESWN(ESWN eswn){return ESWN(eswn+1);}

struct Cell{
    int x,y;Status status;
    ESWN incoming,outgoing;
};

#define LABY_MAX 24
Cell laby[LABY_MAX][LABY_MAX];

inline Cell*neighbor(Cell*cell){
    switch(cell->outgoing){
        case EAST:return cell+LABY_MAX;
        case SOUTH:return cell+1;
        case WEST:return cell-LABY_MAX;
        case NORTH:return cell-1;
        default:return NULL;
    } 
}

inline Cell*advence(Cell*cell){ 
    Cell*next;
    switch(cell->outgoing){
        case EAST:next=cell+LABY_MAX;   next->incoming=WEST;break;
        case SOUTH:next=cell+1;         next->incoming=NORTH;break;
        case WEST:next=cell-LABY_MAX;   next->incoming=EAST;break;
        case NORTH:next=cell-1;         next->incoming=SOUTH;break;
        default:exit(-1);
    }
    return next;
}

bool labyrinth(Cell Laby[LABY_MAX][LABY_MAX],Cell* s,Cell* t){
    if((AVAILABLE!=s->status)||(AVAILABLE!=t->status))return false;
    Stack<Cell*> path;
    s->incoming=UNKNOWN;s->status=ROUTE;path.push(s);
    do{
        Cell* c=path.top();
        if(c==t)return true;
        while(NO_WAY>(c->outgoing=nextESWN(c->outgoing)))
          if(AVAILABLE==(neighbor(c)->status))break;
        if(NO_WAY<=c->outgoing)
          {c->status=BACKTRACKED;path.pop();}
        else
          {path.push(advence(c));c->outgoing=UNKNOWN;c->status=ROUTE;}
    }while(!path.empty());
    return false;
}
