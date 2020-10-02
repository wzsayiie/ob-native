#pragma once

# ifdef __cplusplus
#   error "these are only c headers."
# endif
//use like this if included by c++:
//
//| extern "C" {
//|     #include "xx.h"
//| }

#include "clist.h"
#include "cobj.h"
#include "cont.h"
#include "cstr.h"
#include "cval.h"
#include "dearray.h"
#include "hashmap.h"
#include "imap.h"
#include "io.h"
#include "macro.h"
#include "pod.h"
#include "rbtree.h"
#include "smap.h"
