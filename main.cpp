#include <windows.h>
#include <windowsx.h>
#include "ui.h"

static App app;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_SIZE:
        app.resize(hwnd, LOWORD(lParam), HIWORD(lParam));
        InvalidateRect(hwnd, nullptr, TRUE);
        return 0;

    case WM_LBUTTONDOWN:
        app.click(hwnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        app.paint(hwnd, hdc);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_ERASEBKGND:
        return 1;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int showCommand) {
    const char CLASS_NAME[] = "NetworkAlgorithmVisualizerWindow";

    WNDCLASSA wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = instance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr;

    if (!RegisterClassA(&wc)) {
        MessageBoxA(nullptr, "Could not register the application window.", "Startup Error", MB_ICONERROR);
        return 1;
    }

    HWND hwnd = CreateWindowExA(
        0, CLASS_NAME, "Network Algorithm Visualizer - C++",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1120, 780,
        nullptr, nullptr, instance, nullptr
    );

    if (!hwnd) {
        MessageBoxA(nullptr, "Could not create the application window.", "Startup Error", MB_ICONERROR);
        return 1;
    }

    ShowWindow(hwnd, showCommand);
    UpdateWindow(hwnd);

    MSG msg{};
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return static_cast<int>(msg.wParam);
}
