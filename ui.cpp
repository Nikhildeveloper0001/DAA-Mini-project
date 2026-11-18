#include "ui.h"
#include <windowsx.h>
#include <sstream>
#include <algorithm>

static const COLORREF BG = RGB(16, 22, 34);
static const COLORREF PANEL = RGB(25, 34, 50);
static const COLORREF TEXT = RGB(230, 237, 247);
static const COLORREF MUTED = RGB(151, 166, 187);
static const COLORREF BLUE = RGB(54, 132, 235);
static const COLORREF GREEN = RGB(43, 190, 135);
static const COLORREF ORANGE = RGB(245, 166, 66);
static const COLORREF RED = RGB(225, 85, 95);

App::App()
    : mode(Mode::Select), algorithm(Algorithm::Dijkstra), hasResult(false),
      source(0), destination(1), edgeFirst(-1), selectedNode(-1),
      width(1100), height(720), status("Choose a tool, then click on the network canvas.") {
    int a = graph.addNode(180, 180);
    int b = graph.addNode(390, 120);
    int c = graph.addNode(600, 210);
    int d = graph.addNode(350, 350);
    int e = graph.addNode(650, 410);
    graph.addEdge(a, b, 4);
    graph.addEdge(a, d, 2);
    graph.addEdge(b, c, 3);
    graph.addEdge(d, c, 1);
    graph.addEdge(c, e, 5);
    graph.addEdge(d, e, 7);
}

void App::resize(HWND, int w, int h) {
    width = w;
    height = h;
}

void App::drawText(HDC hdc, int x, int y, const std::string& text, COLORREF color, int size) {
    SetTextColor(hdc, color);
    SetBkMode(hdc, TRANSPARENT);
    HFONT font = CreateFontA(size, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
    HFONT old = (HFONT)SelectObject(hdc, font);
    TextOutA(hdc, x, y, text.c_str(), static_cast<int>(text.size()));
    SelectObject(hdc, old);
    DeleteObject(font);
}

void App::drawButton(HDC hdc, RECT r, const std::string& text, bool active) {
    HBRUSH brush = CreateSolidBrush(active ? BLUE : RGB(43, 55, 74));
    HPEN pen = CreatePen(PS_SOLID, 1, active ? BLUE : RGB(66, 82, 105));
    HGDIOBJ oldBrush = SelectObject(hdc, brush);
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    RoundRect(hdc, r.left, r.top, r.right, r.bottom, 8, 8);
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(brush);
    DeleteObject(pen);
    SIZE s{};
    GetTextExtentPoint32A(hdc, text.c_str(), (int)text.size(), &s);
    drawText(hdc, r.left + (r.right-r.left-s.cx)/2, r.top + 9, text, TEXT, 14);
}

void App::drawNode(HDC hdc, int index, bool visited, bool inPath) {
    const Node& node = graph.getNode(index);
    COLORREF color = inPath ? GREEN : (visited ? ORANGE : BLUE);
    HBRUSH brush = CreateSolidBrush(color);
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(220, 235, 255));
    HGDIOBJ oldBrush = SelectObject(hdc, brush);
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    Ellipse(hdc, node.x-25, node.y-25, node.x+25, node.y+25);
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(brush);
    DeleteObject(pen);
    drawText(hdc, node.x-13, node.y-9, node.name, RGB(255,255,255), 14);
}

int App::findNodeAt(int x, int y) const {
    for (int i = 0; i < graph.nodeCount(); ++i) {
        const Node& n = graph.getNode(i);
        int dx = x - n.x, dy = y - n.y;
        if (dx*dx + dy*dy <= 30*30) return i;
    }
    return -1;
}

void App::runSelectedAlgorithm() {
    if (graph.nodeCount() < 2) {
        status = "Add at least two routers first.";
        return;
    }
    if (source >= graph.nodeCount()) source = 0;
    if (destination >= graph.nodeCount()) destination = graph.nodeCount()-1;

    if (algorithm == Algorithm::BFS) result = runBFS(graph, source, destination);
    else if (algorithm == Algorithm::DFS) result = runDFS(graph, source, destination);
    else result = runDijkstra(graph, source, destination);

    hasResult = true;
    status = result.message;
}

void App::click(HWND hwnd, int x, int y) {
    const int panelX = width - 285;
    if (x >= panelX) {
        int localY = y;
        if (localY >= 100 && localY < 140) { mode = Mode::AddNode; edgeFirst = -1; status = "Click the canvas to add a router."; }
        else if (localY >= 148 && localY < 188) { mode = Mode::AddEdge; edgeFirst = -1; status = "Click two routers to connect them."; }
        else if (localY >= 196 && localY < 236) {
            if (graph.removeLastNode()) { hasResult = false; source = 0; destination = graph.nodeCount() > 1 ? graph.nodeCount()-1 : 0; status = "Last router removed."; }
            else status = "There are no routers to remove.";
        }
        else if (localY >= 244 && localY < 284) { graph.clear(); hasResult = false; source = destination = 0; status = "Network cleared."; }
        else if (localY >= 300 && localY < 340) { algorithm = Algorithm::BFS; status = "BFS selected."; }
        else if (localY >= 348 && localY < 388) { algorithm = Algorithm::DFS; status = "DFS selected."; }
        else if (localY >= 396 && localY < 436) { algorithm = Algorithm::Dijkstra; status = "Dijkstra selected."; }
        else if (localY >= 452 && localY < 500) { runSelectedAlgorithm(); }
        else if (localY >= 520 && localY < 558) {
            if (graph.nodeCount() >= 2) {
                source = (source + 1) % graph.nodeCount();
                status = "Source changed to " + graph.getNode(source).name + ".";
            }
        }
        else if (localY >= 564 && localY < 602) {
            if (graph.nodeCount() >= 2) {
                destination = (destination + 1) % graph.nodeCount();
                status = "Destination changed to " + graph.getNode(destination).name + ".";
            }
        }
    } else {
        int node = findNodeAt(x, y);
        if (mode == Mode::AddNode) {
            int id = graph.addNode(x, y);
            if (id >= 0) status = "Added " + graph.getNode(id).name + ".";
            else status = "Maximum of 20 routers reached.";
            mode = Mode::Select;
            hasResult = false;
        } else if (mode == Mode::AddEdge) {
            if (node >= 0) {
                if (edgeFirst < 0) {
                    edgeFirst = node;
                    status = "First router selected. Click another router.";
                } else {
                    if (graph.addEdge(edgeFirst, node, 3)) status = "Connection added with weight 3.";
                    else status = "Connection not added. It may already exist or be invalid.";
                    edgeFirst = -1;
                    mode = Mode::Select;
                    hasResult = false;
                }
            }
        } else if (node >= 0) {
            selectedNode = node;
            status = graph.getNode(node).name + " selected.";
        }
    }
    InvalidateRect(hwnd, nullptr, TRUE);
}

void App::drawPanel(HDC hdc) {
    RECT panel{width-285, 0, width, height};
    HBRUSH brush = CreateSolidBrush(PANEL);
    FillRect(hdc, &panel, brush);
    DeleteObject(brush);

    int x = width - 265;
    drawText(hdc, x, 24, "CONTROL PANEL", TEXT, 20);
    drawText(hdc, x, 58, "Network Algorithm Visualizer", MUTED, 12);

    drawButton(hdc, {x,100,x+245,140}, "Add Router", mode == Mode::AddNode);
    drawButton(hdc, {x,148,x+245,188}, "Add Connection", mode == Mode::AddEdge);
    drawButton(hdc, {x,196,x+245,236}, "Remove Last Router");
    drawButton(hdc, {x,244,x+245,284}, "Clear Network");

    drawText(hdc, x, 288, "ALGORITHM", MUTED, 12);
    drawButton(hdc, {x,300,x+245,340}, "Breadth First Search (BFS)", algorithm == Algorithm::BFS);
    drawButton(hdc, {x,348,x+245,388}, "Depth First Search (DFS)", algorithm == Algorithm::DFS);
    drawButton(hdc, {x,396,x+245,436}, "Dijkstra Shortest Path", algorithm == Algorithm::Dijkstra);
    drawButton(hdc, {x,452,x+245,500}, "RUN ALGORITHM", true);

    drawText(hdc, x, 506, "ENDPOINTS (cycle selection)", MUTED, 12);
    std::string src = "Source: " + (graph.nodeCount() ? graph.getNode(source < graph.nodeCount() ? source : 0).name : "-");
    std::string dst = "Target: " + (graph.nodeCount() ? graph.getNode(destination < graph.nodeCount() ? destination : 0).name : "-");
    drawButton(hdc, {x,520,x+245,558}, src);
    drawButton(hdc, {x,564,x+245,602}, dst);

    drawText(hdc, x, 616, "RESULT", MUTED, 12);
    std::string info = hasResult ? result.message : "Run an algorithm to see results.";
    drawText(hdc, x, 638, info.substr(0, 34), TEXT, 12);

    if (hasResult) {
        std::string order = "Visit: ";
        for (int id : result.order) order += graph.getNode(id).name + " ";
        drawText(hdc, x, 658, order.substr(0, 36), ORANGE, 12);
        if (result.pathFound) {
            std::string path = "Path: ";
            for (int id : result.path) path += graph.getNode(id).name + " ";
            drawText(hdc, x, 678, path.substr(0, 36), GREEN, 12);
            if (algorithm == Algorithm::Dijkstra)
                drawText(hdc, x, 698, "Total cost: " + std::to_string(result.totalCost), TEXT, 12);
        }
    }
}

void App::paint(HWND, HDC hdc) {
    RECT client{0,0,width,height};
    HBRUSH bg = CreateSolidBrush(BG);
    FillRect(hdc, &client, bg);
    DeleteObject(bg);

    drawText(hdc, 24, 18, "NETWORK ALGORITHM VISUALIZER", TEXT, 22);
    drawText(hdc, 24, 52, "Routers are nodes. Connections are weighted edges.", MUTED, 13);
    drawText(hdc, 24, 78, status, ORANGE, 13);

    // Draw each undirected edge once.
    for (int i = 0; i < graph.nodeCount(); ++i) {
        for (const Edge& edge : graph.neighbors(i)) {
            if (edge.to <= i) continue;
            const Node& a = graph.getNode(i);
            const Node& b = graph.getNode(edge.to);
            bool pathEdge = false;
            if (hasResult && result.pathFound) {
                for (size_t p = 1; p < result.path.size(); ++p) {
                    if ((result.path[p-1] == i && result.path[p] == edge.to) ||
                        (result.path[p-1] == edge.to && result.path[p] == i)) pathEdge = true;
                }
            }
            HPEN pen = CreatePen(PS_SOLID, pathEdge ? 4 : 2, pathEdge ? GREEN : RGB(93,111,137));
            HGDIOBJ old = SelectObject(hdc, pen);
            MoveToEx(hdc, a.x, a.y, nullptr);
            LineTo(hdc, b.x, b.y);
            SelectObject(hdc, old);
            DeleteObject(pen);

            int mx = (a.x+b.x)/2, my = (a.y+b.y)/2;
            RECT label{mx-15,my-12,mx+20,my+12};
            SetBkColor(hdc, BG);
            drawText(hdc, mx-5, my-10, std::to_string(edge.weight), ORANGE, 12);
        }
    }

    for (int i = 0; i < graph.nodeCount(); ++i) {
        bool visited = false, inPath = false;
        if (hasResult) {
            visited = std::find(result.order.begin(), result.order.end(), i) != result.order.end();
            inPath = std::find(result.path.begin(), result.path.end(), i) != result.path.end();
        }
        drawNode(hdc, i, visited, inPath);
    }

    if (graph.nodeCount() > 0) {
        const Node& s = graph.getNode(source < graph.nodeCount() ? source : 0);
        const Node& d = graph.getNode(destination < graph.nodeCount() ? destination : 0);
        drawText(hdc, s.x-20, s.y+31, "SOURCE", GREEN, 10);
        drawText(hdc, d.x-20, d.y+31, "TARGET", ORANGE, 10);
    }

    drawPanel(hdc);
}
