#include "nrunloop.h"
#include "NWin32AppContext.h"

static void ActionProcedure(void *param)
{
    NAction *action = param;
    NActionRun(action, NULL);
    NRelease(action);
}

void NPostOnMain(NAction *action)
{
    if (!action)
    {
        return;
    }

    HWND hwnd = NWINGetMainHWND();
    NRetain(action);
    NWINPostAction(hwnd, ActionProcedure, action);
}
