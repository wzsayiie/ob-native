#pragma once

#include "nmemory.h"

nclass(NLambda, NObject, {
    union {
        NObject Object;
        NObject Super ;
    };
});
