#pragma once

#include "basis.h"

nclass(NTextField, NObject, {
    union {
        NObject Object;
        NObject Super ;
    };
});
