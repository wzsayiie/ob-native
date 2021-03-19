#include "nfuncmeta_p.h"
#include "nstructmeta_p.h"
#include "ntrait_p.h"

#undef  nstruct
#undef  nclass
#define nstruct(nm,     ...) _nstruct(nm,     __VA_ARGS__); AddStruct(nm    ); AddTrait(nm)
#define nclass( nm, sp, ...) _nclass (nm, sp, __VA_ARGS__); AddClass (nm, sp); AddTrait(nm)

#undef  nfunc
#define nfunc(ret, name, args) _nfunc(ret, name, args); AddFunc(name)

#include "NGEN.h"
