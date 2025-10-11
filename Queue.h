#include "List.h"
#include <cstdlib>   // rand()
#include <ctime>     // time()
#include <iostream>  // cout, endl

template<typename T> class Queue{
public:
   void enqueue(T const& e){insertAsLast(e);}
   T dequeue(){return remove(first());}
   T& front(){return first()->data;}

    bool empty() const { return _list.empty(); }
    int size() const { return _list.size(); }
  
private:
    List<T> _list;  // 使用List实现队列

    // 辅助封装
    void insertAsLast(T const& e) { _list.insertAsLast(e); }
    T remove(typename List<T>::Posi p) { return _list.remove(p); }
    typename List<T>::Posi first() { return _list.first(); }
};
struct Customer {
    int window;
    unsigned int time;
};
int bestWindow(Queue<Customer> windows[], int nWin);
bool ServiceClosed();  // 模拟服务是否结束
void Serve(Customer& e);
void RoundRobin() {
   
    const int clients = 10;
    Queue<int> Q;
    for (int i = 0; i < clients; i++) Q.enqueue(i);

    while (!ServiceClosed()) {
        int e = Q.dequeue();
        Serve(*(new Customer{ e % 3, 10 }));
        Q.enqueue(e);
    }
}
struct Customer{int window; unsigned int time;};

void simulate(int nWin,int servTime){
    Queue<Customer>* windows=new Queue<Customer>[nWin];
    for(int now=0;now<servTime;now++){
        if (rand()%(1+nWin)){
            Customer c;c.time=1+rand()%98;
            c.window=bestWindow(windows,nWin);
            windows[c.window].enqueue(c);
        }
        for(int i=0;i<nWin;i++)
          if (!windows[i].empty())
            if(!windows[i].empty())
              if(--windows[i].front().time<=0)
                windows[i].dequeue();
    }
    delete[] windows;
}

int bestWindow(Queue<Customer> windows[],int nWin){
    int minSize=windows[0].size(),optiWin=0;
    for(int i=1;i<nWin;i++)
      if(minSize>windows[i].size())
        {minSize=windows[i].size();optiWin=i;}
    return optiWin;
}