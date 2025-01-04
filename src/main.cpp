#include "libs/logger.hpp"

// ###############################################
//                     Globals
// ###############################################

static bool isRunning = true;

// ###############################################
//                     Functions
// ###############################################

bool platform_create_window(int width, int height, char* title);
void platform_update_window();

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
// Globals
HWND window;
// Functions

LRESULT CALLBACK windows_window_callback(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    switch (msg) {
        // On window close...
        case WM_CLOSE: {
            isRunning = false;
            break;
        }

        // On all other window events (move, minimize, maximize, etc.)
        default: {
            result = DefWindowProcA(window, msg, wParam, lParam);
        }
    }
    return result;
}
bool platform_create_window(int width, int height, char* title)
{
    HINSTANCE instance = GetModuleHandleA(0);
    WNDCLASSA wc = {};
    wc.hInstance = instance;
    wc.hIcon = LoadIcon(instance, IDI_APPLICATION); // Application Icon
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);       // Application Cursor: What style we should have by default
    wc.lpszClassName = title;                       // Application ID: Unique ID for the window
    wc.lpfnWndProc = windows_window_callback;       // Application Callback: Inputs into the Window

    if (!RegisterClassA(&wc)) {
        return false;
    }

    window = CreateWindowExA(0,                   // Window Extended Styles
                             title,               // Class Name
                             title,               // Window Title
                             WS_OVERLAPPEDWINDOW, // Style
                             -1000,               // Position X
                             1100,                // Position Y
                             width,               // Width
                             height,              // Height
                             NULL,                // Parent
                             NULL,                // Menu
                             instance,            // Instance
                             NULL                 // lpParam
    );

    if (window == NULL) {
        return false;
    }

    ShowWindow(window, SW_SHOW);
    return true;
}
void platform_update_window()
{
    MSG msg;
    while (PeekMessageA(&msg, window, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

#endif

int main()
{
    // Create the Window
    platform_create_window(1280, 720, "My Game");

    FP_LOG("Window Created");

    // Main Game Loop
    while (isRunning) {
        // Update the Window
        platform_update_window();

        // FP_LOG("Hello");
        // FP_WARN("Hello");
        // FP_ERROR("Hello");
        // FP_ASSERT(true, "Assertion Hit");
    }

    // Return 0 when game loop is done (exitting appliation)
    return 0;
}