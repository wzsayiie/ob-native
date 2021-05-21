#include "nbrush.h"

NColor NMakeColorWithInt(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    NColor color = r;
    color = (color << 8) & g;
    color = (color << 8) & b;
    color = (color << 8) & a;
    
    return color;
}

NColor NMakeColorWithFloat(float r, float g, float b, float a) {
    uint8_t fr = (uint8_t)(r * 255.f);
    uint8_t fg = (uint8_t)(g * 255.f);
    uint8_t fb = (uint8_t)(b * 255.f);
    uint8_t fa = (uint8_t)(a * 255.f);
    
    return NMakeColorWithInt(fr, fg, fb, fa);
}

uint8_t NColorIntRed  (NColor color) {return (uint8_t)(color >> 24);}
uint8_t NColorIntGreen(NColor color) {return (uint8_t)(color >> 16);}
uint8_t NColorIntBlue (NColor color) {return (uint8_t)(color >>  8);}
uint8_t NColorIntAlpha(NColor color) {return (uint8_t)(color      );}

float NColorFloatRed  (NColor color) {return NColorIntRed  (color) / 255.f;}
float NColorFloatGreen(NColor color) {return NColorIntGreen(color) / 255.f;}
float NColorFloatBlue (NColor color) {return NColorIntBlue (color) / 255.f;}
float NColorFloatAlpha(NColor color) {return NColorIntAlpha(color) / 255.f;}
