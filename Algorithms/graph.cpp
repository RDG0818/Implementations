/*
Basic implementation of fundamental graph algorithms.

Depth-First Search: O(V + E)

Breadth-First Search: O(V + E)

Djikstra's Algorithm: O((V + E)log V) or O(V^2)

Bellman Ford: O(V * E)

Prim's Algorithm: O((V + E)log V) or O(V^2)

Kruskal's Algorithm: O(E log E) or O(E log V)

*/

#include <iostream>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

void dfs(int starting_node) {
    stack<int> st;
    
}

void bfs() {

}

void djikstra() {

}

void bellman_ford() {

}

void prim() {

}

void kruskal() {

}

int main() {
int N; // Nodes
int M; // Edges

cin >> N >> M;

vector<vector<int>> unweighted_graph(N);

// Will be given M lines, each having u and v, indicating an edge between vertex u and v

int u, v;
for (int i = 0; i < M; i++) {
    cin >> u >> v;
    unweighted_graph[u].push_back(v); unweighted_graph[v].push_back(u);
}


return 0;
}
