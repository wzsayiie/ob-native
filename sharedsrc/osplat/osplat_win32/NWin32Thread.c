#include "nthread.h"
#include <Windows.h>

static DWORD WINAPI ThreadProcedure(LPVOID data)
{
    NAction *action = data;
    NActionRun(action, NULL);
    NRelease(action);
    return 0;
}

void NRunThread(NAction *action)
{
    if (action)
    {
        NRetain(action);
        CreateThread(NULL, 0, ThreadProcedure, action, 0, NULL);
    }
}

void NThreadSleep(float seconds)
{
    DWORD milliseconds = (DWORD)(1000 * seconds);
    Sleep(milliseconds);
}
