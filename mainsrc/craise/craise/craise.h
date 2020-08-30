#pragma once

# ifdef __cplusplus
#   error "these are only c headers."
# endif
//use like this if included by c++:
//
//| extern "C" {
//|     #include "xx.h"
//| }

#include <stdbool.h>
#include <stdint.h>

#include "io.h"
#include "macro.h"
#include "ustr.h"
#include "xlist.h"
#include "xmap.h"
