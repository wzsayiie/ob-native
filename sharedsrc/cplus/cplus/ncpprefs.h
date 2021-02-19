#pragma once

#include "nobjectref.h"

ncppclass(NData  , NDataRef  , NObjectRef) {NCPP_BODY};
ncppclass(NNumber, NNumberRef, NObjectRef) {NCPP_BODY};
ncppclass(NString, NStringRef, NObjectRef) {NCPP_BODY};

ncppclass(NWordArray, NWordArrayRef, NObjectRef   ) {NCPP_BODY};
ncppclass(NArray    , NArrayRef    , NWordArrayRef) {NCPP_BODY};
ncppclass(NIntArray , NIntArrayRef , NWordArrayRef) {NCPP_BODY};
ncppclass(NFltArray , NFltArrayRef , NWordArrayRef) {NCPP_BODY};

ncppclass(NWordTable, NWordTableRef, NObjectRef   ) {NCPP_BODY};
ncppclass(NTable    , NTableRef    , NWordTableRef) {NCPP_BODY};
ncppclass(NIntTable , NIntTableRef , NWordTableRef) {NCPP_BODY};
ncppclass(NStrTable , NStrTableRef , NWordTableRef) {NCPP_BODY};
