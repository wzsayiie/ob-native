#include "nthread.h"
#include <Windows.h>

static DWORD WINAPI _NThreadProcedure(LPVOID data)
{
    NAction *action = (NAction *)data;
    NActionRun(action);
    NRelease(action);
    return 0;
}

void NRunThread(NAction *action)
{
    if (action)
    {
        NRetain(action);
        CreateThread(NULL, 0, _NThreadProcedure, action, 0, NULL);
    }
}

void NThreadSleep(float seconds)
{
    DWORD milliseconds = (DWORD)(1000 * seconds);
    Sleep(milliseconds);
}
