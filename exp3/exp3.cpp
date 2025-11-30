#include <iostream>
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
 Vertex(Tv const& d = Tv()) :
    data(d), inDegree(0), outDegree(0), status(UNDISCOVERED), 
    dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) {}
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
    int n, e;
public:
    GraphMatrix() {
        n =e = 0;
    }


    ~GraphMatrix(){
        for(int j=0;j<n;j++)
            for(int k=0;k<n;k++)
                delete E[j][k];
    }

    virtual Tv& vertex(int i){return V[i].data;}
    virtual int inDegree(int i){return V[i].inDegree;}
    virtual int outDegree(int i){return V[i].outDegree;}
    virtual int firstNbr(int i){return nextNbr(i,n);}
    virtual int nextNbr(int i, int j) {
    while (--j >= 0 && !exists(i, j));
    return j;
    }
    virtual VStatus& status(int i) { return V[i].status; }

    virtual int& dTime(int i){return V[i].dTime;}
    virtual int& fTime(int i){return V[i].fTime;}
    virtual int& parent(int i){return V[i].parent;}
    virtual int& priority(int i){return V[i].priority;}

    virtual int insert(Tv const& vertex){

        for (int j = 0; j < n; j++) {
            E[j].insert(nullptr);  // 修正为nullptr，表示没有边
        }
        n++; 
        
        E.insert(Vector< Edge<Te>* >(n,n,nullptr));
        return V.insert(Vertex<Tv>(vertex));

        V.insert(Vertex<std::string>(vertex)); // 插入一个字符串类型的顶点
        return n++; 
    }
    virtual Tv remove(int i){
        for(int j=0;j<n;j++)
          if(exists(i,j)){delete E[i][j];V[j].inDegree--;}
        E.remove(i);n--;
        Tv vBak = vertex(i);V.remove(i);
        for(int j=0;j<n;j++)
          if(Edge<Te>*e=E[j].remove(i)){delete e;V[j].outDegree--;}
        return vBak;
    }

    virtual bool exists(int i,int j)
    {return(0<=i)&&(i<n)&&(0<=j)&&(j<n)&&E[i][j]!=NULL;}

    virtual EType& type(int i,int j){return E[i][j]->type;}
    virtual Te& edge (int i,int j){return E[i][j]->data;}
    virtual int& weight(int i,int j){return E[i][j]->weight;}

    virtual void insert(Te const& edge,int w,int i,int j){
        if(exists(i,j)) return;
        E[i][j]=new Edge<Te> (edge,w);
        e++,V[i].outDegree++;V[j].inDegree++;
    }
    virtual Te remove (int i,int j){
        Te eBak =edge (i,j);delete E[i][j];E[i][j]=NULL;
        e--;V[i].outDegree--;V[j].inDegree--;  // 修正语法错误
        return eBak;
    }
    void printAdjMatrix();
    void printShortestPaths(int s); // 新增
    void printMST();                // 新增
    void printBCC();
    void printTraversal();
};

template<typename Tv, typename Te>
void GraphMatrix<Tv, Te>::printAdjMatrix() {
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

template<typename Tv, typename Te>
void GraphMatrix<Tv, Te>::printShortestPaths(int s) {
    cout << "Shortest paths from " << vertex(s) << ":" << endl;
    for (int i = 0; i < n; ++i) {
        if (i != s)
            cout << "To " << vertex(i) << ": " << priority(i) << endl;
    }
}
template<typename Tv, typename Te>
void GraphMatrix<Tv, Te>::printMST() {
    cout << "Minimum Spanning Tree edges:" << endl;
    for (int i = 0; i < n; ++i) {
        if (parent(i) != -1)          // 0 以上才是树边
            cout << vertex(parent(i)) << " - " << vertex(i)
                 << " (" << weight(parent(i), i) << ")" << endl;
    }
}

template<typename Tv, typename Te>
void GraphMatrix<Tv, Te>::printBCC() {
    cout << "Biconnected Components and Articulation Points:" << endl;
    // TODO: 遍历你 bcc() 里记录的 component / cut 数组并打印
}
template<typename Tv, typename Te>
void GraphMatrix<Tv, Te>::printTraversal() {
    for (int i = 0; i < n; i++) {
        cout << vertex(i) << "(" << dTime(i) << "/" << fTime(i) << ") ";
        if (parent(i) != -1)
            cout << "parent: " << vertex(parent(i)) << " ";
        cout << endl;
    }
}
int main() {
    // 创建图1
    GraphMatrix<string, int> g1;

// 插入节点，传递字符串作为节点数据
    g1.insert("A");
    g1.insert("B");
    g1.insert("C");
    g1.insert("D");
    g1.insert("E");
    g1.insert("F");
    g1.insert("G");
    g1.insert("H");
    
    // 插入边
    g1.insert(4, 1, 0, 1);  // 从节点 0 到节点 1，边的数据是 4，权重是 1
    g1.insert(6, 2, 0, 3);  // 从节点 0 到节点 3，边的数据是 6，权重是 2
    g1.insert(12, 3, 0, 2); // 从节点 0 到节点 2，边的数据是 12，权重是 3
    g1.insert(9, 1, 1, 2);  // 从节点 1 到节点 2，边的数据是 9，权重是 1
    g1.insert(1, 2, 2, 4);  // 从节点 2 到节点 4，边的数据是 1，权重是 2
    g1.insert(2, 3, 2, 5);  // 从节点 2 到节点 5，边的数据是 2，权重是 3
    g1.insert(13, 4, 3, 4); // 从节点 3 到节点 4，边的数据是 13，权重是 4
    g1.insert(7, 5, 3, 6);  // 从节点 3 到节点 6，边的数据是 7，权重是 5
    g1.insert(5, 6, 4, 5);  // 从节点 4 到节点 5，边的数据是 5，权重是 6
    g1.insert(8, 7, 4, 7);  // 从节点 4 到节点 7，边的数据是 8，权重是 7
    g1.insert(10, 8, 5, 7); // 从节点 5 到节点 7，边的数据是 10，权重是 8
    g1.insert(14, 9, 6, 7); // 从节点 6 到节点 7，边的数据是 14，权重是 9
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
 // 在任务3部分，应该这样调用：
    cout << "\nDijkstra's algorithm from A:" << endl;
    g1.pfs(0, DijkstraPU<string, int>());
    g1.printShortestPaths(0);

    cout << "\nPrim's algorithm from A:" << endl;
    g1.pfs(0, PrimPU<string, int>());
    g1.printMST();
    
    // 任务4
    cout << "\nBiconnected Components:" << endl;
    g1.bcc(0);
    g1.printBCC();
    
    return 0;
}