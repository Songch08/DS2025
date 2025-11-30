#include"Vector.h"
#include"Graph.h"

template<typename Tv> struct Vertex { 
    Tv data;int inDegree, outDegree;VStatus status;
    int dTime, fTime;
    int parent;int priority;
    Vertex(Tv const& d = (Tv) 0) :
       data (d),inDegree (0), outDegree (0), status (UNDISCOVERED), 
       dTime (-1), fTime (-1), parent (-1), priority (INT_MAX);{}
};

template<typename Te> struct Edge { 
    Te data;int weight;EType type;
    Edge(Te const& d, int w ) :data (d), weight (w), type (UNDETERMINED);{}
};

template<typename Tv, typename Te> 
class GraphMatrix : public Graph<Tv, Te> { 
private:
    Vector< Vertex<Tv> > V;
    Vector< Vector< Edge<Te>* > > E;
public:
    GraphMatrix(){n=e=0;}
    ~GraphMatrix(){
        for(int j=0;j<n;j++)
            for(int k=0;k<n;k++)
                delete E[j][k];
    }

    virtual Tv& vertex(int i){return V[i].data;}
    virtual int inDegree(int i){return V[i].inDegree;}
    virtual int outDegree(int i){return V[i].outDegree;}
    virtual int firstNbr(int i){return nextNbr(1,n);}
    virtual int nextNbr(int i,int j)
    {while ((-1<j)&&(!exists(i,--j)))return j;}
    virtual VStatus& status(int i){return V[i].status;}
    virtual int& dTime(int i){return V[i].dTime;}
    virtual int& fTime(int i){return V[i].fTime;}
    virtual int& parent(int i){return V[i].parent;}
    virtual int& priority(int i){return V[i].priority;}

    virtual int insert(Tv const& vertex){
        for(int j=0;j<n;j++) E[j].insert(NULL);n++; 
        E.insert(Vector< Edge<Te>* >(n,n,(Edge<Te>*)NULL));
        return V.insert(Vertex<Tv>(vertex));
    }
    virtual Tv remove(int i){
        for(int j=0;j<n;j++)
          if(exists(i,j)){delete E[i][j];V[j].inDegree--;}
        E.remove(1);n--;
        Tv vBak = vertex(i);V.remove(i);
        for(int j=0;j<n;j++)
          if(Edge<Te>*e=E[j].remove(i)){delete e;V[j].outDegree--;}
        return vBak;
    }

    virtual bool exists(int i,int j)
    {return(0<=i)&&(i<0)&&(0<=j)&&(j<n)&&E[i][j]!=NULL;}

    virtual EType& type(int i,int j){return E[i][j]->type;}
    virtual Te& edge (int i,int j){return E[i][j]->data;}
    virtual int& weight(int i,int j){return E[i][j]->weight;}

    virtual void insert(Te const& edge,int w,int i,int j){
        if(exists(i,j)) return;
        E[i][j]=new Edge<Te> (edge,w);
        e++,V[i].outDegree++;V[j]inDegree++;
    }
    virtual Te remove (int i,int j){
        Te eBak =edge (i,j);delete E[i][j]=NULL;
        e--;V[i].outDegree--;V{j}.inDegree--;
        return eBak;
    }
};

template <typename Tv,typename Te>
void Graph<Tv,Te>::bfs (int s){
  reset();int clock=0;int v=s;
  do
    if(UNDISCOVERED == status(v))
      BFS (v,clock);
  while (s !=(v=(++v%n)));
}

template <typename Tv,typename Te>
void Graph<Tv,Te>::BFS (int v,int& clock){
  Queue<int> Q;
  status(v)=DISCOVERED;Q.enqueue(v);
  while(!Q.empty()){
    int v =Q.dequeue();dTime(v)=++clock;
    for (int u=firstNbr(v);-1<u;u=nextNbr(v,u))
      if(UNDISCOVERED==status(u)){
        status(u)=DISCOVERED;Q.enqueue(u);
        type (v,u)=TREE;parent(u)=v;
      }else{
        type(v,u)=CROSS;
      }
    status(v)=VISITED;
  }
}

template <typename Tv,typename Te>
void Graph<Tv,Te>::dfs (int s){
  reset();int clock=0;int v =s;
  do 
    if (UNDISCOVERED==status(v))
      DFS(v,clock);
  while(s!=(v=(++v%n)));
}

template <typename Tv,typename Te>
void Graph<Tv,Te>::DFS (int v,int& clock){
  dTime (v)=++clock;status(v)=DISCOVERED;
  for (int u=firstNbr(v);-1<u;u=nextNbr(v,u))
    switch(status(u)){
      case UNDISCOVERED:
        type(v,u)=TREE;parent(u)=v;DFS(u,clock);break;
      case DISCOVERED:
        type (v,u)=BACKWARD;break;
      default:
        type(v,u)=(dTime(v)<dTime(u))?FORWARD:CROSS;break;
    }
  status(v)=VISITED;fTime(v)=++clock;
}

template <typename Tv,typename Te>
Stack<Tv>* Graph<Tv,Te>::tSort(int s){
  reset();int clock=0;int v=s;
  Stack<Tv>* S=new Stack<Tv>;
  do{
    if(UNDISCOVERED==status(v))
      if(!TSort(v,clock,S)){
        while (!S->empty())
          S->pop();break;
      }
  }while(s!=(v=(++v%n)));
  return S;
}

template <typename Tv,typename Te>
bool Graph<Tv,Te>::TSort(int v,int& clock,Stack<Tv>* S){
  dTime(v)=++clock;status(v)=DISCOVERED;
  for(int u=firstNbr(v);-1<u;u=nextNbr(v,u))
    switch (status(u)){
      case UNDISCOVERED:
        parent(u)=v;type (v,u)=TREE;
        if(!TSort(u,clock,S))
          return false;
        break;
      case DISCOVERED:
        type(v,u)=BACKWARD;
        return false;
      default:
      type(v,u)=(dTime(v)<dTime(u))?FORWARD:CROSS;
        break;
    }
  status(v)=VISITED;S->push(vertex(v));
  return true;
}

template <typename Tv,typename Te> void Graph<Tv,Te>::bcc (int s){
  reset();int clock=0;int v=s;Stack<int> S;
  do 
    if(UNDISCOVERED==status(v)){
      BCC(v,clock,S);
      S.pop();
    }
  while (s!=(v=(++v%n)));
}
#define hca(x) (fTime(x))
template <typename Tv,typename Te>
void Graph<Tv,Te>::BCC(int v,int& clock,Stack<int>& S){
  hca(v)=dTime(v)=++clock;status(v)=DISCOVERED;S.push(v);
  for(int u=firstNbr(v);-1<u;u=nextNbr(v,u))
    switch (status(u)){
      case UNDISCOVERED:
        parent(u)=v;type(v,u)=TREE;BCC(u,clock,S);
        if(hca(u)<dTime(v))
          hca(v)=min(hca(v),hca(u));
        else{
          while(v!=S.pop());
          S.push(v);
        }
        break;
      case DISCOVERED:
        type(v,u)=BACKWARD;
        if(u!=parent(v)) hca(v)=min(hca(v),dTime(u));
        break;
      default:
        type(v,u)=(dTime(v)<dTime(u))?FORWARD:CROSS;
        break;
    }
  status(v)=VISITED;
}
#undef hca

template <typename Tv,typename Te> template <typename PU>
void Graph<Tv,Te>::pfs(int s,PU prioUpdater){
  reset();int v=s;
  do 
    if(UNDISCOVERED==status(v))
      PFS(v,prioUpdater);
  while (s!=(v=(++v%n)));
}

template <typename Tv,typename Te> template <typename PU>
void Graph<Tv,Te>::PFS(int s,PU prioUpdater){
  priority(s)=0;status(s)=VISITED;parent(s)=-1;
  while(1){
    for(int w=firstNbr(s);-1<w;w=nextNbr(s,w))
      prioUpdater(this,s,w);
    for(int shortest=INT_MAX,w=0;w<n;w++)
      if(UNDISCOVERED==status(w))
        if(shortest>priority(w))
          {shortest=priority(w);s=w;}
    if (VISITED==status(s))break;
    status(s)=VISITED;type(parent(s),s)=TREE;
  }
}

template <typename Tv,typename Te> struct PrimPU{
  virtual void operator()(Graph<Tv,Te>* g,int uk,int v){ 
    if(UNDISCOVERED==g->status(v))
      if(g->priority(v)>g->weight(uk,v)){
        g->priority(v)=g->weight(uk,v);
        g->parent(v)=uk;
      }
  }
};

template <typename Tv,typename Te> struct DijkstraPU{
  virtual void operator()(Graph<Tv,Te>* g,int uk,int v){ 
    if(UNDISCOVERED==g->status(v))
      if(g->priority(v)>g->priority(uk)+g->weight(uk,v)){
        g->priority(v)=g->priority(uk)+g->weight(uk,v);
        g->parent(v)=uk;
      }
  }
};