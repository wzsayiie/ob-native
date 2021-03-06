#pragma once

#include <Windows.h>
#include <stdbool.h>

//NOTE: ui host need call these functions.
void _NWINSetMainHWND(HWND hwnd);
bool _NWINWindowProc(UINT msg, WPARAM wParam, LPARAM lParam);

//user functions:

HWND NWINGetMainHWND(void);

typedef void (*NWINActionFunc)(void *arg);
void NWINPostAction(HWND hwnd, NWINActionFunc func, void *arg);
