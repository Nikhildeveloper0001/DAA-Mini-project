@echo off
setlocal
echo Building Network Algorithm Visualizer...
g++ -std=c++17 -O2 -Wall -Wextra main.cpp graph.cpp algorithms.cpp ui.cpp -o NetworkAlgorithmVisualizer.exe -mwindows -lgdi32 -luser32
if errorlevel 1 (
    echo.
    echo Build failed. Check that MinGW g++ is installed and added to PATH.
    pause
    exit /b 1
)
echo.
echo Build successful. Starting application...
NetworkAlgorithmVisualizer.exe
pause
