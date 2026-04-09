#include <windows.h>

static WNDPROC originalWndProc = NULL;

static LRESULT CALLBACK HookWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // Swallow "you lost focus" messages so the game never knows
    if (msg == WM_ACTIVATE && LOWORD(wParam) == WA_INACTIVE) return 0;
    if (msg == WM_KILLFOCUS) return 0;
    if (msg == WM_ACTIVATEAPP && wParam == FALSE) return 0;
    return CallWindowProc(originalWndProc, hwnd, msg, wParam, lParam);
}

static BOOL CALLBACK FindMainWindow(HWND hwnd, LPARAM lParam) {
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid == GetCurrentProcessId() && IsWindowVisible(hwnd) && GetParent(hwnd) == NULL) {
        *(HWND*)lParam = hwnd;
        return FALSE;
    }
    return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        Sleep(500); // give the window a moment to exist
        HWND gameWindow = NULL;
        EnumWindows(FindMainWindow, (LPARAM)&gameWindow);
        if (gameWindow) {
            originalWndProc = (WNDPROC)SetWindowLongPtr(
                gameWindow, GWLP_WNDPROC, (LONG_PTR)HookWndProc);
        }
    }
    return TRUE;
}