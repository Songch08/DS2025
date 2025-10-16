#include "Stack.h"

int nCheck = 0;   
int nSolu = 0;    
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
};
void placeQueens( int N){
    Stack<Queen> solu;
    Queen q(0,0);
    do{
        if (N<=solu.size()||N<=q.y){
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