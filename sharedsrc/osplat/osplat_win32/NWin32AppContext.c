#include "NWin32AppContext.h"

static const UINT NWM_LAMBDA = WM_USER + 0x401;

static HWND sMainHWND = NULL;

void _NWINSetMainHWND(HWND hwnd)
{
    sMainHWND = hwnd;
}

bool _NWINWindowProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
    /*
    if (msg == NWM_LAMBDA)
    {
        NLambda *lambda = (NLambda *)wParam;
        NLambdaPrepareCall(lambda);
        NLambdaCallVoid(lambda);
        NRelease(lambda);

        return true;
    }
    */
    return false;
}

HWND NWINGetMainHWND(void)
{
    return sMainHWND;
}

/*
void NWINPostLambda(HWND hwnd, NLambda *lambda)
{
    if (hwnd && lambda)
    {
        NRetain(lambda);
        PostMessageW(hwnd, NWM_LAMBDA, (WPARAM)lambda, 0);
    }
}
*/
