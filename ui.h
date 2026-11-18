#ifndef UI_H
#define UI_H

#include <windows.h>
#include "graph.h"
#include "algorithms.h"
#include <string>

enum class Mode {
    Select,
    AddNode,
    AddEdge
};

enum class Algorithm {
    BFS,
    DFS,
    Dijkstra
};

class App {
public:
    App();
    void paint(HWND hwnd, HDC hdc);
    void click(HWND hwnd, int x, int y);
    void resize(HWND hwnd, int width, int height);

private:
    Graph graph;
    Mode mode;
    Algorithm algorithm;
    AlgorithmResult result;
    bool hasResult;
    int source;
    int destination;
    int edgeFirst;
    int selectedNode;
    int width;
    int height;
    std::string status;

    void runSelectedAlgorithm();
    int findNodeAt(int x, int y) const;
    void drawText(HDC hdc, int x, int y, const std::string& text, COLORREF color, int size = 16);
    void drawButton(HDC hdc, RECT rect, const std::string& text, bool active = false);
    void drawNode(HDC hdc, int index, bool visited, bool inPath);
    void drawPanel(HDC hdc);
};

#endif
