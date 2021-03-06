#include "NWin32Main.h"

int APIENTRY wWinMain(
    _In_     HINSTANCE inst    ,
    _In_opt_ HINSTANCE prevInst,
    _In_     LPWSTR    cmdLine ,
    _In_     int       cmdShow )
{
    return NWINMain(inst, prevInst, cmdLine, cmdShow);
}
