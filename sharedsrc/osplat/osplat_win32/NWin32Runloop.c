#include "nrunloop.h"
#include "NWin32AppContext.h"

void NPostOnMain(NLambda *lambda)
{
    HWND hwnd = NWINGetMainHWND();
    NWINPostLambda(hwnd, lambda);
}
