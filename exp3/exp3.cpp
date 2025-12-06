#include <iostream>
#include <queue>
#include <climits>
#include <stack>
#include <algorithm>
#include <functional>
#include <windows.h> 
#include "../Vector.h"
using namespace std;
class Graph {
public:
    int V;  
    vector<vector<int>> adjMatrix;  
    Graph(int V) {
        this->V = V;
        adjMatrix.resize(V, vector<int>(V, 0));  
    }
    void addEdge(int u, int v, int weight = 1) {
        adjMatrix[u][v] = weight;
        adjMatrix[v][u] = weight;  
    }
    void printAdjMatrix() {
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                cout << adjMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }
};
void BFS(Graph& g, int start) {
    vector<bool> visited(g.V, false);
    queue<int> q;
    visited[start] = true;
    q.push(start);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        cout << v << " ";
        for (int i = 0; i < g.V; ++i) {
            if (g.adjMatrix[v][i] != 0 && !visited[i]) {
                visited[i] = true;
                q.push(i);
            }
        }
    }
    cout << endl;
}
void DFS(Graph& g, int start, vector<bool>& visited) {
    visited[start] = true;
    cout << start << " ";
    for (int i = 0; i < g.V; ++i) {
        if (g.adjMatrix[start][i] != 0 && !visited[i]) {
            DFS(g, i, visited);
        }
    }
}
void Dijkstra(Graph& g, int start) {
    vector<int> dist(g.V, INT_MAX);
    dist[start] = 0;
    vector<bool> visited(g.V, false);
    for (int i = 0; i < g.V; ++i) {
        int u = -1;
        for (int j = 0; j < g.V; ++j) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }
        visited[u] = true;
        for (int v = 0; v < g.V; ++v) {
            if (g.adjMatrix[u][v] != 0 && dist[u] + g.adjMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + g.adjMatrix[u][v];
            }
        }
    }
    cout << "最短路径: ";
    for (int i = 0; i < g.V; ++i) {
        cout << "从 " << start << " 到 " << i << " 的最短路径为: " << dist[i] << endl;
    }
}
class UnionFind {
public:
    vector<int> parent, rank;
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int x) {
        if (x != parent[x]) parent[x] = find(parent[x]);
        return parent[x];
    }
    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) {
            if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            } else if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            } else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
        }
    }
};
void Kruskal(Graph& g) {
    vector<pair<int, pair<int, int>>> edges;
    for (int i = 0; i < g.V; ++i) {
        for (int j = i + 1; j < g.V; ++j) {
            if (g.adjMatrix[i][j] != 0) {
                edges.push_back({g.adjMatrix[i][j], {i, j}});
            }
        }
    }
    sort(edges.begin(), edges.end());
    UnionFind uf(g.V);
    cout << "最小生成树的边: ";
    for (auto& edge : edges) {
        int u = edge.second.first;
        int v = edge.second.second;
        if (uf.find(u) != uf.find(v)) {
            uf.unite(u, v);
            cout << "(" << u << ", " << v << ") ";
        }
    }
    cout << endl;
}
void findBiconnectedComponents(Graph& g) {
    vector<bool> visited(g.V, false);
    vector<int> disc(g.V, -1), low(g.V, -1), parent(g.V, -1);
    stack<pair<int, int>> st;
    int time = 0;
    std::function<void(int)> dfs = [&](int u) {
        visited[u] = true;
        disc[u] = low[u] = time++;
        for (int v = 0; v < g.V; ++v) {
            if (g.adjMatrix[u][v] == 0) continue;
            if (!visited[v]) {
                parent[v] = u;
                st.push({u, v});
                dfs(v);
                low[u] = min(low[u], low[v]);
                if (low[v] >= disc[u]) {
                    cout << "关节点: " << u << endl;
                    while (st.top() != make_pair(u, v)) {
                        cout << "(" << st.top().first << ", " << st.top().second << ") ";
                        st.pop();
                    }
                    cout << "(" << st.top().first << ", " << st.top().second << ") ";
                    st.pop();
                }
            } else if (v != parent[u] && disc[v] < disc[u]) {
                low[u] = min(low[u], disc[v]);
                st.push({u, v});
            }
        }
    };
    for (int i = 0; i < g.V; ++i) {
        if (!visited[i]) {
            dfs(i);
        }
    }
}
int main() {
    SetConsoleOutputCP(CP_UTF8);
    Graph g(8);  
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 6);
    g.addEdge(1, 2, 0);
    g.addEdge(1, 3, 9);
    g.addEdge(2, 4, 12);
    g.addEdge(2, 5, 2);
    g.addEdge(3, 4, 7);
    g.addEdge(4, 5, 5);
    g.addEdge(4, 6, 11);
    g.addEdge(4, 7, 8);
    g.addEdge(5, 6, 10);
    g.addEdge(5, 7, 3);
    g.addEdge(6, 7, 14);
    cout << "图 1 邻接矩阵: " << endl;
    g.printAdjMatrix();
    cout << endl;
    cout << "BFS: ";
    BFS(g, 0);
    cout << endl;
    cout << "DFS: ";
    vector<bool> visited(g.V, false);
    DFS(g, 0, visited);
    cout << endl;
    Dijkstra(g, 0);
    Kruskal(g);
    cout << "双连通分量关节点和边: " << endl;
    findBiconnectedComponents(g);
    return 0;
}
