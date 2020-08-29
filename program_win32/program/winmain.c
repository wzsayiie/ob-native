#include "UDWinMain.h"

int APIENTRY wWinMain(
    _In_     HINSTANCE hInst    ,
    _In_opt_ HINSTANCE hPrevInst,
    _In_     LPWSTR    lpCmdLine,
    _In_     int       nCmdShow )
{
    return UDWinMain(hInst, hPrevInst, lpCmdLine, nCmdShow);
}
