# Network Algorithm Visualizer (C++)

A beginner-friendly native Windows desktop application that demonstrates graph traversal and shortest-path algorithms using a visual network of routers.

## Algorithms
- **BFS (Breadth-First Search):** visits reachable routers level by level.
- **DFS (Depth-First Search):** explores one branch before backtracking.
- **Dijkstra:** calculates the minimum total edge-weight path for non-negative connection weights.

## Requirements
- Windows
- MinGW-w64 with `g++`
- VS Code (optional)

No third-party GUI or networking libraries are required. The interface uses the Windows API and GDI.

## Build and run
1. Extract the project folder.
2. Open the folder in VS Code or Command Prompt.
3. Make sure `g++ --version` works.
4. Double-click `build.bat`, or run:

```bash
g++ -std=c++17 -O2 -Wall -Wextra main.cpp graph.cpp algorithms.cpp ui.cpp -o NetworkAlgorithmVisualizer.exe -mwindows -lgdi32 -luser32
```

5. Run `NetworkAlgorithmVisualizer.exe`.

## How to use
- The sample network is loaded automatically.
- Click **Add Router**, then click an empty place on the canvas.
- Click **Add Connection**, then click two different routers. New connections start with weight 3.
- Select BFS, DFS, or Dijkstra.
- Click **Run Algorithm**.
- The visited routers are highlighted orange and the resulting route is highlighted green.
- The Source and Target buttons cycle through the available routers.
- **Remove Last Router** removes the most recently added router.
- **Clear Network** removes all routers and connections.

## Notes and limitations
- The graph is undirected.
- The graph supports up to 20 routers.
- Connection weights are positive integers.
- The current UI adds connections with weight 3. To change a weight, update the `addEdge` call in `ui.cpp` or extend the UI with a weight-entry dialog.
- This project visualizes graph algorithms; it does not send real network packets or configure actual routers.
- BFS and DFS produce traversal-based routes, not necessarily minimum-cost routes. Dijkstra finds the minimum-weight route.

## Source files
- `main.cpp`: Windows application entry point and message loop.
- `graph.h/.cpp`: graph data structure and router/link operations.
- `algorithms.h/.cpp`: BFS, DFS, and Dijkstra implementations.
- `ui.h/.cpp`: drawing, controls, and mouse interaction.
- `build.bat`: compile and launch helper.
