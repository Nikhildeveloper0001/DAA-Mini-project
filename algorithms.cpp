#include "algorithms.h"
#include <queue>
#include <stack>
#include <algorithm>
#include <limits>
#include <functional>

static std::vector<int> makePath(const std::vector<int>& parent, int source, int destination) {
    std::vector<int> path;
    if (destination < 0 || destination >= static_cast<int>(parent.size())) return path;

    int current = destination;
    while (current != -1) {
        path.push_back(current);
        if (current == source) break;
        current = parent[current];
    }

    if (path.empty() || path.back() != source) return {};
    std::reverse(path.begin(), path.end());
    return path;
}

AlgorithmResult runBFS(const Graph& graph, int source, int destination) {
    AlgorithmResult result{};
    result.totalCost = 0;
    result.pathFound = false;

    int n = graph.nodeCount();
    if (source < 0 || destination < 0 || source >= n || destination >= n) {
        result.message = "Choose valid source and destination routers.";
        return result;
    }

    std::vector<bool> visited(n, false);
    std::vector<int> parent(n, -1);
    std::queue<int> q;

    visited[source] = true;
    q.push(source);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        result.order.push_back(u);

        if (u == destination) break;

        for (const Edge& edge : graph.neighbors(u)) {
            if (!visited[edge.to]) {
                visited[edge.to] = true;
                parent[edge.to] = u;
                q.push(edge.to);
            }
        }
    }

    result.path = makePath(parent, source, destination);
    result.pathFound = !result.path.empty();
    result.message = result.pathFound ? "BFS traversal and route completed." : "Destination is not reachable.";
    return result;
}

AlgorithmResult runDFS(const Graph& graph, int source, int destination) {
    AlgorithmResult result{};
    result.totalCost = 0;
    result.pathFound = false;

    int n = graph.nodeCount();
    if (source < 0 || destination < 0 || source >= n || destination >= n) {
        result.message = "Choose valid source and destination routers.";
        return result;
    }

    std::vector<bool> visited(n, false);
    std::vector<int> parent(n, -1);
    std::stack<int> st;
    st.push(source);

    while (!st.empty()) {
        int u = st.top();
        st.pop();

        if (visited[u]) continue;
        visited[u] = true;
        result.order.push_back(u);

        if (u == destination) break;

        const auto& edges = graph.neighbors(u);
        for (auto it = edges.rbegin(); it != edges.rend(); ++it) {
            if (!visited[it->to]) {
                if (parent[it->to] == -1 && it->to != source) parent[it->to] = u;
                st.push(it->to);
            }
        }
    }

    result.path = makePath(parent, source, destination);
    result.pathFound = !result.path.empty();
    result.message = result.pathFound ? "DFS traversal and route completed." : "Destination is not reachable.";
    return result;
}

AlgorithmResult runDijkstra(const Graph& graph, int source, int destination) {
    AlgorithmResult result{};
    result.totalCost = 0;
    result.pathFound = false;

    int n = graph.nodeCount();
    if (source < 0 || destination < 0 || source >= n || destination >= n) {
        result.message = "Choose valid source and destination routers.";
        return result;
    }

    const int INF = std::numeric_limits<int>::max() / 4;
    result.distance.assign(n, INF);
    std::vector<int> parent(n, -1);
    using Item = std::pair<int, int>;
    std::priority_queue<Item, std::vector<Item>, std::greater<Item>> pq;

    result.distance[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        int distance = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (distance != result.distance[u]) continue;
        result.order.push_back(u);
        if (u == destination) break;

        for (const Edge& edge : graph.neighbors(u)) {
            int candidate = distance + edge.weight;
            if (candidate < result.distance[edge.to]) {
                result.distance[edge.to] = candidate;
                parent[edge.to] = u;
                pq.push({candidate, edge.to});
            }
        }
    }

    result.path = makePath(parent, source, destination);
    result.pathFound = !result.path.empty();
    if (result.pathFound) {
        result.totalCost = result.distance[destination];
        result.message = "Shortest weighted path found using Dijkstra.";
    } else {
        result.message = "Destination is not reachable.";
    }
    return result;
}
