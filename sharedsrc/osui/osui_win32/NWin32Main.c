#include "NWin32Main.h"
#include "NWin32AppContext.h"
#include "osui.h"
#include <stdio.h>

static void OpenConsole(void)
{
    AllocConsole();

    FILE* newStdin  = NULL;
    FILE* newStdout = NULL;
    FILE* newStderr = NULL;
    _wfreopen_s(&newStdin , L"conin$" , L"r", stdin );
    _wfreopen_s(&newStdout, L"conout$", L"w", stdout);
    _wfreopen_s(&newStderr, L"conout$", L"w", stderr);

    WCHAR title[MAX_PATH];
    GetConsoleTitleW(title, MAX_PATH);

    HWND hWnd = FindWindowW(NULL, title);
    SetWindowPos(hWnd, HWND_TOP, 300, 100, 0, 0, SWP_NOSIZE);
}

static LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_CREATE)
    {
        _NWINSetMainHWND(hwnd);
        OpenConsole();
        NLaunch();
    }
    else if (msg == WM_PAINT)
    {
        PAINTSTRUCT paint;
        BeginPaint(hwnd, &paint);
        EndPaint(hwnd, &paint);
    }
    else if (msg == WM_DESTROY)
    {
        PostQuitMessage(0);
    }
    else
    {
        bool consumed = _NWINWindowProc(msg, wParam, lParam);
        if (consumed) {
            return 0;
        }
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }

    return 0;
}

int NWINMain(HINSTANCE inst, HINSTANCE prevInst, LPWSTR cmdLine, int cmdShow)
{
    //register main window class.
    LPCWSTR className = L"NMainWindow";
    WNDCLASSW wndClass = {0};

    wndClass.style         = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc   = MainWindowProc;
    wndClass.cbClsExtra    = 0;
    wndClass.cbWndExtra    = 0;
    wndClass.hInstance     = inst;
    wndClass.hIcon         = NULL;
    wndClass.hCursor       = NULL;
    wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wndClass.lpszMenuName  = NULL;
    wndClass.lpszClassName = className;

    RegisterClassW(&wndClass);

    //show main window.
    HWND hwnd = CreateWindowExW(
        /* dwExStyle    */ 0,
        /* lpClassName  */ className,
        /* lpWindowName */ L"Native",
        /* dwStyle      */ WS_OVERLAPPEDWINDOW,
        /* x,y          */   0, 100,
        /* width,height */ 300, 300,
        /* hWndParent   */ NULL,
        /* hMenu        */ NULL,
        /* hInstance    */ inst,
        /* lpParam      */ NULL
    );

    ShowWindow(hwnd, cmdShow);
    UpdateWindow(hwnd);

    //message loop.
    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}
