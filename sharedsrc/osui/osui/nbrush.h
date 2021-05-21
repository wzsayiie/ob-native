#pragma once

#include "nimage.h"

//NColor:

typedef uint32_t NColor;

nfunc(NColor, NMakeColorWithInt  , (uint8_t r, uint8_t g, uint8_t b, uint8_t a));
nfunc(NColor, NMakeColorWithFloat, (float   r, float   g, float   b, float   a));

nfunc(uint8_t, NColorIntRed  , (NColor color));
nfunc(uint8_t, NColorIntGreen, (NColor color));
nfunc(uint8_t, NColorIntBlue , (NColor color));
nfunc(uint8_t, NColorIntAlpha, (NColor color));

nfunc(float, NColorFloatRed  , (NColor color));
nfunc(float, NColorFloatGreen, (NColor color));
nfunc(float, NColorFloatBlue , (NColor color));
nfunc(float, NColorFloatAlpha, (NColor color));

//NBrush:

nclass(NBrush, NObject, {
    union {
        NObject Object;
        NObject Super ;
    };
});
