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
    
};