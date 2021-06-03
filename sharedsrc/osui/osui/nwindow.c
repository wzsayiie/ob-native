#include "nwindow.h"

/*
static const int DEFAULT_MS_TICK = 100;

void _NWindowInit  (NWindow *window) {_NObjectInit  (nsuperof(window));}
void _NWindowDeinit(NWindow *window) {_NObjectDeinit(nsuperof(window));}

NWindow *_NWindowCreate(void) {
    NWindow *window = NAlloc(NWindow, _NWindowDeinit);
    _NWindowInit(window);
    return window;
}

NWindow *NMainWindow(void) {
    static NWindow *window = NULL;
    for_once() {
        window = _NWindowCreate();
    }
    return window;
}

void NWindowSetTick(NWindow *window, int milliseconds) {
    if (window) {
        window->msTick = milliseconds;
    }
}

void NWindowSetListener(NWindow *window, NLambda *listener) {
    if (window) {
        NRelease(window->listener);
        window->listener = NRetain(listener);
    }
}

NBrush *NWindowBrush(NWindow *window) {
    return NULL;
}

float NWindowTouchX(NWindow *wnd) {return wnd ? wnd->touchX : 0;}
float NWindowTouchY(NWindow *wnd) {return wnd ? wnd->touchY : 0;}
float NWindowWidth (NWindow *wnd) {return wnd ? wnd->width  : 0;}
float NWindowHeight(NWindow *wnd) {return wnd ? wnd->height : 0;}

void NWindowAddTextField(NWindow *window, NTextField *textField) {
}

void NWindowRemoveTextField(NWindow *window, NTextField *textField) {
}

int _NWindowTick(NWindow *window) {
    if (window) {
        return window->msTick > 0 ? window->msTick : DEFAULT_MS_TICK;
    }
    return 0;
}

void _NWindowSendEvent(NWindow *window, NWindowEvent event) {
    if (window && window->listener) {
        NLambda *listener = window->listener;
        
        NLambdaPrepareCall(listener);
        NLambdaPushInt(listener, event);
        NLambdaCallVoid(listener);
    }
}

void _NWindowSetTouch(NWindow *window, float x, float y) {
    if (window) {
        window->touchX = x;
        window->touchY = y;
    }
}

void _NWindowSetSize(NWindow *window, float width, float height) {
    if (window) {
        window->width  = width ;
        window->height = height;
    }
}
*/
