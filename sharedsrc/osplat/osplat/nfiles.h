#pragma once

#include "basis.h"

nfunc(NString *, NCopyDocumentPath , (void));
nfunc(NString *, NCopyCachesPath   , (void));
nfunc(NString *, NCopyTemporaryPath, (void));

nfunc(bool    , NMakeDirectory, (NString *path, bool intermediate));
nfunc(NArray *, NCopySubitems , (NString *path, bool *success));
nfunc(bool    , NPathExists   , (NString *path, bool *isDirectory));
nfunc(void    , NRemovePath   , (NString *path));
