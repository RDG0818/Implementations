/*
Basic implementation of fundamental graph algorithms.

Depth-First Search: O(V + E)
Uses: Path Finding, Directed/Undirected Cycle Detection, Bipartite Checking, Connected Components, Topological Sort, Kosaraju

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
#include <algorithm>

using namespace std;

// DFS Algorithms

void dfs_path_finding(vector<vector<int>>& graph, int starting_node, int target_node) {
    stack<int> st;
    st.push(starting_node);
    vector<bool> visited(graph.size(), false);
    vector<int> prev(graph.size(), -1);
    visited[starting_node] = true;
    while(!st.empty()) {
        int curr_node = st.top();
        st.pop();
        if (curr_node == target_node) {
            cout << "Path Exists" << endl;
            int node = target_node;
            vector<int> path = {node};
            while (node != starting_node) {
                path.push_back(prev[node]);
                node = prev[node];
            }
            for (int i = path.size() - 1; i >= 0; i--) {
                cout << path[i] << " ";
            }
            return;
        }
        for (int neighbor : graph[curr_node]) {
            if (!visited[neighbor]) {
                st.push(neighbor);
                visited[curr_node] = true;
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
            visited[starting_node] = true;
            while(!st.empty()) {
                int curr_node = st.top();
                st.pop();
                for (int neighbor : graph[curr_node]) {
                    if (!visited[neighbor]) {
                        st.push(neighbor);
                        visited[neighbor] = true;
                        prev[neighbor] = curr_node;
                    }
                    else if (visited[neighbor] && neighbor != prev[curr_node]) {
                        found_cycle = true; // Can break here if you do not need to find components
                    }
                }
            }
        }
    }

    if (!found_cycle) cout << "No Cycle Found" << endl;
    else cout << "Found Cycle" << endl;
    cout << "Total Components: " << components << endl;
}

void directed_cycle_check(vector<vector<int>>& graph) {
    stack<pair<int, int>> st;
    vector<int> visit_state(graph.size(), 0);
    bool found_cycle = false;
    for (int starting_node = 0; starting_node < graph.size() && !found_cycle; starting_node++) {
        if (visit_state[starting_node] == 0 && !found_cycle) {
            st.push(pair(starting_node, 0));
            visit_state[starting_node] = 1;
            while (!st.empty()) {
                auto& [curr_node, idx] = st.top();

                if (idx < (int)graph[curr_node].size()) {
                    int neighbor = graph[curr_node][idx++];
                    if (visit_state[neighbor] == 0) {
                        st.push(pair(neighbor, 0));
                        visit_state[neighbor] = 1;
                    }
                    else if (visit_state[neighbor] == 1) { // back edge detected
                        found_cycle = true;
                        break;
                    } 
                }
                else {
                    visit_state[curr_node] = 2;
                    st.pop();
                }
            }
        }
    }
    if (!found_cycle) cout << "No Cycle Found" << endl;
    else cout << "Found Cycle" << endl;
}

void bipartite_check(vector<vector<int>>& graph) {
    stack<int> st;
    vector<int> color(graph.size(), -1);
    bool bipartite = true;
    for (int starting_node = 0; starting_node < graph.size(); starting_node++) {
        if (color[starting_node] == -1) {
            st.push(starting_node);
            color[starting_node] = 0;
            while (!st.empty() && bipartite) {
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

void topological_sort(vector<vector<int>>& graph) { // Assumes graph is directed and acyclic
    stack<pair<int, int>> st;
    vector<int> visit_state(graph.size(), 0);
    vector<int> sort_order;
    for (int starting_node = 0; starting_node < graph.size(); starting_node++) {
        if (visit_state[starting_node] == 0) {
            st.push(pair(starting_node, 0));
            visit_state[starting_node] = 1;
            while (!st.empty()) {
                auto& [curr_node, idx] = st.top();
                if (idx < (int)graph[curr_node].size()) {
                    int neighbor = graph[curr_node][idx++];
                    if (visit_state[neighbor] == 0) {
                        st.push(pair(neighbor, 0));
                        visit_state[neighbor] = 1;
                    }
                }
                else {
                    st.pop();
                    visit_state[curr_node] = 2;
                    sort_order.push_back(curr_node);
                }
            }
        }
    }
    reverse(sort_order.begin(), sort_order.end());
    for (int node : sort_order) {
        cout << node << " ";
    }
    cout << endl;

}

void kosaraju(vector<vector<int>>& graph) {

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

vector<vector<int>> graph(N);

// Will be given M lines, each having u && v, indicating an edge between vertex u && v

int u, v;
for (int i = 0; i < M; i++) {
    cin >> u >> v;
    graph[u].push_back(v); 
}

// dfs_path_finding(unweighted_graph, 0, 3); 
// undirected_cycle_check(unweighted_graph);
// bipartite_check(unweighted_graph);
directed_cycle_check(graph);
topological_sort(graph);
return 0;
}
