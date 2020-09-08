#pragma once

#include "macro.h"

//the including is necessary.
//visual studio use it to determine whether to link the library.
//this can also check whether the declarations of rewritten functions
//is consistent with the standard's.
#include <stdio.h>

# if ON_WINDOWS
#   include <sal.h>                 //_In_z_
                                    //_Printf_format_string_
#   define __printflike(fmt, args)  //__printflike
# else
#   define _In_z_                   //_In_z_
#   define _Printf_format_string_   //_Printf_format_string_
#   include <sys/cdefs.h>           //__printflike
# endif

//if the link order of custom "printf" is higher,
//it will replace the function with same name from "stdio".
int printf(_In_z_ _Printf_format_string_ const char *format, ...) __printflike(1, 2);
