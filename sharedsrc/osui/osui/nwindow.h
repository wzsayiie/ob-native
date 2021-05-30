#pragma once

#include "nbrush.h"
#include "ntextfield.h"

/*
nclass(NWindow, NObject, {
    union {
        NObject Object;
        NObject Super ;
    };

    int      msTick;
    NLambda *listener;
    float    touchX;
    float    touchY;
    float    width ;
    float    height;
});

void _NWindowInit  (NWindow *window);
void _NWindowDeinit(NWindow *window);

NWindow *_NWindowCreate(void);

nfunc(NWindow *, NMainWindow, (void));

nfunc(void, NWindowSetTick, (NWindow *window, int milliseconds));

nenum(NWindowEvent, {
    
    //the lifecycle events.
    NWE_LOAD   = 1,
    NWE_SHOW   = 2,
    NWE_HIDE   = 3,
    NWE_UNLOAD = 4,
    
    //the clock tick count.
    NWE_TICK = 5,
    
    //the graphics update event.
    //get the "brush" by "NWindowGetBrush", and draw graphs.
    NWE_DRAW = 6,
    
    //the mouse click and finger touch event.
    //get touch position by "NWindowGetTouchX" and "NWindowGetTouchY".
    NWE_TOUCH_BEGIN = 7,
    NWE_TOUCH_MOVE  = 8,
    NWE_TOUCH_END   = 9,
    
    //the window resize event.
    //get new size by "NWindowGetWidth" and "NWindowHetHeight".
    NWE_RESIZE = 10,
});

nfunc(void, NWindowSetListener, (NWindow *window, NLambda *listener));

nfunc(NBrush *, NWindowBrush , (NWindow *window));
nfunc(float   , NWindowTouchX, (NWindow *window));
nfunc(float   , NWindowTouchY, (NWindow *window));
nfunc(float   , NWindowWidth , (NWindow *window));
nfunc(float   , NWindowHeight, (NWindow *window));

nfunc(void, NWindowAddTextField   , (NWindow *window, NTextField *textField));
nfunc(void, NWindowRemoveTextField, (NWindow *window, NTextField *textField));

int  _NWindowTick     (NWindow *window);
void _NWindowSendEvent(NWindow *window, NWindowEvent event);
void _NWindowSetTouch (NWindow *window, float x, float y);
void _NWindowSetSize  (NWindow *window, float width, float height);
*/
