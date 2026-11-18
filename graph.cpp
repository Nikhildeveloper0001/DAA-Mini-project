#include "graph.h"
#include <algorithm>

Graph::Graph() {}

int Graph::addNode(int x, int y) {
    if (nodes.size() >= MAX_NODES) return -1;

    Node node;
    node.x = x;
    node.y = y;
    node.name = "R" + std::to_string(nodes.size() + 1);

    nodes.push_back(node);
    adjacency.push_back(std::vector<Edge>());
    return static_cast<int>(nodes.size()) - 1;
}

bool Graph::addEdge(int from, int to, int weight) {
    if (from < 0 || to < 0 || from >= nodeCount() || to >= nodeCount()) return false;
    if (from == to || weight < 1 || weight > 999 || hasEdge(from, to)) return false;

    adjacency[from].push_back({to, weight});
    adjacency[to].push_back({from, weight});
    return true;
}

bool Graph::removeLastNode() {
    if (nodes.empty()) return false;
    int removed = nodeCount() - 1;
    nodes.pop_back();
    adjacency.pop_back();

    for (auto& edges : adjacency) {
        edges.erase(std::remove_if(edges.begin(), edges.end(),
            [removed](const Edge& edge) { return edge.to == removed; }), edges.end());
    }
    return true;
}

void Graph::clear() {
    nodes.clear();
    adjacency.clear();
}

int Graph::nodeCount() const {
    return static_cast<int>(nodes.size());
}

const Node& Graph::getNode(int index) const {
    return nodes.at(index);
}

const std::vector<Edge>& Graph::neighbors(int index) const {
    return adjacency.at(index);
}

bool Graph::hasEdge(int from, int to) const {
    if (from < 0 || to < 0 || from >= nodeCount() || to >= nodeCount()) return false;
    for (const Edge& edge : adjacency[from]) {
        if (edge.to == to) return true;
    }
    return false;
}

bool Graph::setEdgeWeight(int from, int to, int weight) {
    if (weight < 1 || weight > 999 || !hasEdge(from, to)) return false;

    for (Edge& edge : adjacency[from]) if (edge.to == to) edge.weight = weight;
    for (Edge& edge : adjacency[to]) if (edge.to == from) edge.weight = weight;
    return true;
}
