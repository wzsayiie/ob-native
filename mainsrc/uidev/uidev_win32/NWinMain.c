#include "NWinMain.h"
#include <stdio.h>
#include "uidev.h"

static VOID OpenConsole(VOID)
{
    AllocConsole();

    FILE* lpStdin  = NULL;
    FILE* lpStdout = NULL;
    FILE* lpStderr = NULL;
    _wfreopen_s(&lpStdin , L"conin$" , L"r", stdin );
    _wfreopen_s(&lpStdout, L"conout$", L"w", stdout);
    _wfreopen_s(&lpStderr, L"conout$", L"w", stderr);

    WCHAR szTitle[MAX_PATH];
    GetConsoleTitleW(szTitle, MAX_PATH);

    HWND hWnd = FindWindow(NULL, szTitle);
    SetWindowPos(hWnd, HWND_TOP, 300, 100, 0, 0, SWP_NOSIZE);
}

static LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
    switch (nMsg)
    {
        case WM_CREATE:
        {
            break;
        }
        
        case WM_PAINT:
        {
            PAINTSTRUCT stPaint;
            BeginPaint(hWnd, &stPaint);
            EndPaint(hWnd, &stPaint);
            break;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }

        default:
        {
            return DefWindowProc(hWnd, nMsg, wParam, lParam);
        }
    }
    return 0;
}

int NWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow)
{
    OpenConsole();

    NLaunch();

    //register main window class.
    LPCWSTR lpszClassName = L"UDMainWindow";
    WNDCLASSW stClass;

    stClass.style         = CS_HREDRAW | CS_VREDRAW;
    stClass.lpfnWndProc   = MainWindowProc;
    stClass.cbClsExtra    = 0;
    stClass.cbWndExtra    = 0;
    stClass.hInstance     = hInst;
    stClass.hIcon         = NULL;
    stClass.hCursor       = NULL;
    stClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
    stClass.lpszMenuName  = NULL;
    stClass.lpszClassName = lpszClassName;

    RegisterClassW(&stClass);

    //show main window.
    HWND hWnd = CreateWindowW(
        /* lpClassName  */ lpszClassName,
        /* lpWindowName */ L"Native",
        /* dwStyle      */ WS_OVERLAPPEDWINDOW,
        /* x,y          */   0, 100,
        /* width,height */ 300, 300,
        /* hWndParent   */ NULL,
        /* hMenu        */ NULL,
        /* hInstance    */ hInst,
        /* lpParam      */ NULL
    );

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    //message loop.
    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}
