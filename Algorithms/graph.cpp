/*
Basic implementation of fundamental graph algorithms.

Depth-First Search: O(V + E)
Uses: Path Finding, Cycle Detection, Bipartite Checking, Connected Components, Topological Sort

Breadth-First Search: O(V + E)

Djikstra's Algorithm/A*: O((V + E)log V) or O(V^2)

Bellman Ford: O(V * E)

Prim's Algorithm: O((V + E)log V) or O(V^2)

Kruskal's Algorithm: O(E log E) or O(E log V)

*/

#include <iostream>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

// DFS Algorithms

void dfs_path_finding(vector<vector<int>>& graph, int starting_node, int target_node) {
    stack<int> st;
    st.push(starting_node);
    vector<bool> visited(graph.size(), false);
    vector<int> prev(graph.size(), -1);
    while(!st.empty()) {
        int curr_node = st.top();
        st.pop();
        if (curr_node == target_node) {
            cout << "Path Exists" << endl;
            int node = target_node;
            vector<int> path = {node};
            while (node != -1) {
                path.push_back(prev[node]);
                node = prev[node];
            }
            for (int i = path.size() - 1; i >= 0; i--) {
                cout << path[i] << " ";
            }
            return;
        }
        if (visited[curr_node]) continue;
        visited[curr_node] = true;
        for (int neighbor : graph[curr_node]) {
            if (!visited[neighbor]) {
                st.push(neighbor);
                prev[neighbor] = curr_node;
            }
        }
    }
    cout << "No Path Exists";
}

void undirected_cycle_check(vector<vector<int>>& graph) {
    stack<int> st;
    vector<int> visited(graph.size(), false);
    vector<int> prev(graph.size(), -1);
    bool found_cycle = false;
    int components = 0;
    for (int starting_node = 0; starting_node < graph.size(); starting_node++) {
        if (!visited[starting_node]) {
            components++;
            st.push(starting_node);
            while(!st.empty() and !found_cycle) {
                int curr_node = st.top();
                st.pop();
                if (visited[curr_node]) continue;
                visited[curr_node] = true;
                for (int neighbor : graph[curr_node]) {
                    if (!visited[neighbor]) {
                        st.push(neighbor);
                        prev[neighbor] = curr_node;
                    }
                    else if (visited[neighbor] and neighbor != prev[curr_node]) {
                        found_cycle = true;
                        break;
                    }
                }
            }
        }
    }

    if (!found_cycle) cout << "No Cycle Found" << endl;
    else cout << "Found Cycle" << endl;
    cout << "Total Components: " << components << endl;
}

void bipartite_check(vector<vector<int>>& graph) {
    stack<int> st;
    vector<int> color(graph.size(), -1);
    bool bipartite = true;
    for (int starting_node = 0; starting_node < graph.size(); starting_node++) {
        if (color[starting_node] == -1) {
            st.push(starting_node);
            color[starting_node] = 0;
            while (!st.empty() and bipartite) {
                int curr_node = st.top();
                st.pop();
                for (int neighbor : graph[curr_node]) {
                    if (color[neighbor] == -1) {
                        st.push(neighbor);
                        color[neighbor] = 1 - color[curr_node];
                    }
                    else if (color[neighbor] == color[curr_node]) {
                        bipartite = false;
                        break;
                    }
                }
            }
        }
        if (!bipartite) break;
    }
    if (bipartite) cout << "Graph is bipartite" << endl;
    else cout << "Graph is not bipartite" << endl;
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


undirected_cycle_check(unweighted_graph);
bipartite_check(unweighted_graph);

return 0;
}
