#pragma once

#include <Windows.h>
#include "basis.h"

//NOTE: ui host need call these functions.
void _NWINSetMainHWND(HWND hwnd);
bool _NWINWindowProc(UINT msg, WPARAM wParam, LPARAM lParam);

//user functions.
HWND NWINGetMainHWND(void);
void NWINPostLambda(HWND hwnd, NLambda *lambda);
