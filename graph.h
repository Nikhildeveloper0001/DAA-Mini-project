#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

struct Edge {
    int to;
    int weight;
};

struct Node {
    int x;
    int y;
    std::string name;
};

class Graph {
public:
    static const int MAX_NODES = 20;

    Graph();
    int addNode(int x, int y);
    bool addEdge(int from, int to, int weight);
    bool removeLastNode();
    void clear();
    int nodeCount() const;
    const Node& getNode(int index) const;
    const std::vector<Edge>& neighbors(int index) const;
    bool hasEdge(int from, int to) const;
    bool setEdgeWeight(int from, int to, int weight);

private:
    std::vector<Node> nodes;
    std::vector<std::vector<Edge>> adjacency;
};

#endif
