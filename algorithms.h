#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "graph.h"
#include <vector>
#include <string>

struct AlgorithmResult {
    std::vector<int> order;
    std::vector<int> path;
    std::vector<int> distance;
    int totalCost;
    bool pathFound;
    std::string message;
};

AlgorithmResult runBFS(const Graph& graph, int source, int destination);
AlgorithmResult runDFS(const Graph& graph, int source, int destination);
AlgorithmResult runDijkstra(const Graph& graph, int source, int destination);

#endif
