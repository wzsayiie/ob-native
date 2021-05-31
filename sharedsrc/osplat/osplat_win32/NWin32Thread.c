#include "nthread.h"
#include <Windows.h>

static DWORD WINAPI ThreadProcedure(LPVOID data)
{
    /*
    NLambda *lambda = data;
    NLambdaPrepareCall(lambda);
    NLambdaCallVoid(lambda);
    NRelease(lambda);
    */
    return 0;
}

void NRunThread(int64_t param, NThreadFunc func)
{
    /*
    if (lambda)
    {
        NRetain(lambda);
        CreateThread(NULL, 0, ThreadProcedure, lambda, 0, NULL);
    }
    */
}

void NThreadSleep(float seconds)
{
    DWORD milliseconds = (DWORD)(1000 * seconds);
    Sleep(milliseconds);
}
