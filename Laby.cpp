#include "Stack.h"
#include <cstdlib> 
typedef enum {AVAILABLE,ROUTE,BACKTRACKED,WALL }Status;

typedef enum {UNKNOWN,EAST,SOUTH,WEST,NORTH,NO_WAY}ESWN;

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
        default:exit(-1);
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
