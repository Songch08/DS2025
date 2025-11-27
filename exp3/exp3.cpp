#include <iostream>
#include <vector>
#include <climits>
#include "../Vector.h"
#include "../Graph.h"
#include "../Stack.h"
#include "../Queue.h"
#include <string>
using namespace std;

template<typename Tv> struct Vertex { 
    Tv data;int inDegree, outDegree;VStatus status;
    int dTime, fTime;
    int parent;int priority;
    Vertex(Tv const& d = (Tv) 0) :
       data (d),inDegree (0), outDegree (0), status (UNDISCOVERED), 
       dTime (-1), fTime (-1), parent (-1), priority (INT_MAX){}
};

template<typename Te> struct Edge { 
    Te data;int weight;EType type;
    Edge(Te const& d, int w ) :data (d), weight (w), type (UNDETERMINED){}
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

void printAdjMatrix() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (exists(i, j)) {
                cout << weight(i, j) << "\t";
            } else {
                cout << "0\t";
            }
        }
        cout << endl;
    }
}
void printShortestPaths(int s) {
    cout << "Shortest paths from " << vertex(s) << ":" << endl;
    for (int i = 0; i < n; i++) {
        if (i != s) {
            cout << "To " << vertex(i) << ": " << priority(i) << endl;
        }
    }
}

void printMST() {
    cout << "Minimum Spanning Tree edges:" << endl;
    for (int i = 0; i < n; i++) {
        if (parent(i) != -1) {
            cout << vertex(parent(i)) << " - " << vertex(i) 
                 << " (" << weight(parent(i), i) << ")" << endl;
        }
    }
}
void printBCC() {
    cout << "Biconnected Components and Articulation Points:" << endl;
    // 实现具体输出逻辑
}
int main() {
    // 创建图1
    GraphMatrix<string, int> g1;
    
    // 插入节点
    g1.insert("A"); g1.insert("B"); g1.insert("C");
    g1.insert("D"); g1.insert("E"); g1.insert("F");
    g1.insert("G"); g1.insert("H");
    
    // 插入边
    g1.insert(4, 0, 1); g1.insert(6, 0, 3); g1.insert(12, 0, 2);
    g1.insert(9, 1, 2); g1.insert(1, 2, 4); g1.insert(2, 2, 5);
    g1.insert(13, 3, 4); g1.insert(7, 3, 6); g1.insert(5, 4, 5);
    g1.insert(8, 4, 7); g1.insert(10, 5, 7); g1.insert(14, 6, 7);
    
    // 任务1
    cout << "Graph 1 adjacency matrix:" << endl;
    g1.printAdjMatrix();
    
    // 任务2
    cout << "\nBFS from A:" << endl;
    g1.bfs(0);
    g1.printTraversal();
    
    cout << "\nDFS from A:" << endl;
    g1.dfs(0);
    g1.printTraversal();
    
    // 任务3
    cout << "\nDijkstra's algorithm from A:" << endl;
    g1.dijkstra(0);
    g1.printShortestPaths(0);
    
    cout << "\nPrim's algorithm from A:" << endl;
    g1.prim(0);
    g1.printMST();
    
    // 任务4
    cout << "\nBiconnected Components:" << endl;
    g1.bcc(0);
    g1.printBCC();
    
    return 0;
}