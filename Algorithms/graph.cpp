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

void undirected_cycle_check(vector<vector<int>>& graph) { // Assumes graph is undirected
    vector<int> visited(graph.size(), false);
    vector<int> prev(graph.size(), -1);
    bool found_cycle = false;
    int components = 0;
    for (int starting_node = 0; starting_node < graph.size(); starting_node++) {
        if (visited[starting_node]) continue;
        components++;
        stack<int> st;
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

    if (!found_cycle) cout << "No Cycle Found" << endl;
    else cout << "Found Cycle" << endl;
    cout << "Total Components: " << components << endl;
}

void directed_cycle_check(vector<vector<int>>& graph) { // Assumes graph is directed
    vector<int> visit_state(graph.size(), 0);
    bool found_cycle = false;
    for (int starting_node = 0; starting_node < graph.size() && !found_cycle; starting_node++) {
        if (visit_state[starting_node] != 0) continue; 
        stack<pair<int, int>> st;
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
    if (!found_cycle) cout << "No Cycle Found" << endl;
    else cout << "Found Cycle" << endl;
}

void bipartite_check(vector<vector<int>>& graph) { // Assumes graph is undirected
    vector<int> color(graph.size(), -1);
    bool bipartite = true;
    for (int starting_node = 0; starting_node < graph.size(); starting_node++) {
        if (color[starting_node] != -1) continue;
        stack<int> st;
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
        if (!bipartite) break;
    }
    if (bipartite) cout << "Graph is bipartite" << endl;
    else cout << "Graph is not bipartite" << endl;
}

void topological_sort(vector<vector<int>>& graph) { // Assumes graph is directed and acyclic
    vector<int> visit_state(graph.size(), 0);
    vector<int> sort_order;
    for (int starting_node = 0; starting_node < graph.size(); starting_node++) {
        if (visit_state[starting_node] != 0) continue;
        stack<pair<int, int>> st;
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
    reverse(sort_order.begin(), sort_order.end());
    for (int node : sort_order) {
        cout << node << " ";
    }
    cout << endl;

}

void kosaraju(vector<vector<int>>& graph) { // Assumes graph is directed
    vector<bool> visit_state(graph.size(), false);
    vector<int> post_order;

    for (int starting_node = 0; starting_node < graph.size(); starting_node++) {
        if (visit_state[starting_node] != 0) continue;
        stack<pair<int, int>> st;
        st.push(pair(starting_node, 0));
        visit_state[starting_node] = true;
        while (!st.empty()) {
            auto&[curr_node, idx] = st.top();

            if (idx < (int)graph[curr_node].size()) {
                int neighbor = graph[curr_node][idx++];
                if (visit_state[neighbor] == 0) {
                    st.push(pair(neighbor, 0));
                    visit_state[neighbor] = true;
                }
            }
            else {
                post_order.push_back(curr_node);
                st.pop();
            }
        }
    }

    reverse(post_order.begin(), post_order.end());
    vector<int> component_id(graph.size(), -1);
    int num_components = 0;

    vector<vector<int>> reversed_graph(graph.size());
    for (int i = 0; i < graph.size(); i++) {
        for (int j = 0; j < graph[i].size(); j++) {
            reversed_graph[graph[i][j]].push_back(i);
        }
    }

    for (int starting_node : post_order) {
        if (component_id[starting_node] != -1) continue;
        stack<pair<int, int>> st;
        component_id[starting_node] = num_components++;
        st.push(pair(starting_node, 0));
        while (!st.empty()) {
            auto&[curr_node, idx] = st.top();
            if (idx < (int)reversed_graph[curr_node].size()) {
               int neighbor = reversed_graph[curr_node][idx++]; 
               if (component_id[neighbor] == -1) {
                   st.push(pair(neighbor, 0));
                   component_id[neighbor] = component_id[curr_node];
               }
            }
            else {
                st.pop();
            }
        }
    }
    cout << "Number of SCC: " << num_components << endl;
    cout << "Component IDs" << endl; 
    for (int i = 0; i < component_id.size(); i++) cout << "Node: " << i << " | ID: " << component_id[i] << endl;
}

void bfs(vector<vector<int>>& graph, int start_node, int end_node) {
    vector<int> distance(graph.size(), -1); distance[start_node] = 0;
    vector<int> prev(graph.size(), -1);
    queue<int> q; q.push(start_node);
    while (!q.empty()) {
        int curr_node = q.front();
        q.pop();
        for (int neighbor : graph[curr_node]) {
            if (distance[neighbor] == -1) {
                distance[neighbor] = distance[curr_node] + 1;
                prev[neighbor] = curr_node;
                q.push(neighbor);
            }
        }
    }
    stack<int> st;
    int curr_node = end_node;
    while(curr_node != -1) {
        st.push(curr_node);
        curr_node = prev[curr_node];
    }

    cout << "Distance from " << start_node << " to " << end_node << ": " << distance[end_node] << endl;
    cout << "Path:" << endl;
    while(!st.empty()) {
        cout << st.top() << " ";
        st.pop();
    }
    cout << endl;
    
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
    graph[u].push_back(v); graph[v].push_back(u);   
}

// dfs_path_finding(unweighted_graph, 0, 3); 
// undirected_cycle_check(unweighted_graph);
// bipartite_check(unweighted_graph);
bfs(graph, 0, 5);
return 0;
}
