#include "NWin32AppContext.h"

static const UINT NWM_ACTION = WM_USER + 0x401;

static HWND sMainHWND = NULL;

void _NWINSetMainHWND(HWND hwnd)
{
    sMainHWND = hwnd;
}

bool _NWINWindowProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == NWM_ACTION)
    {
        NWINActionFunc func = (NWINActionFunc)wParam;
        void *arg = (void *)lParam;
        func(arg);

        return true;
    }
    return false;
}

HWND NWINGetMainHWND(void)
{
    return sMainHWND;
}

void NWINPostAction(HWND hwnd, NWINActionFunc func, void *arg)
{
    if (!hwnd || !func)
    {
        return;
    }

    WPARAM wParam = (WPARAM)func;
    LPARAM lParam = (LPARAM)arg;
    PostMessageW(hwnd, NWM_ACTION, wParam, lParam);
}
