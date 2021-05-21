#pragma once

#include "basis.h"

nclass(NImage, NObject, {
    union {
        NObject Object;
        NObject Super ;
    };
});
