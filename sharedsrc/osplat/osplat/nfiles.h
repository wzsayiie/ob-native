#pragma once

#include "basis.h"

nfunc(NString *, NCopyDocumentPath , (void));
nfunc(NString *, NCopyCachesPath   , (void));
nfunc(NString *, NCopyTemporaryPath, (void));

nfunc(bool    , NMakeDirectory, (NString *path, bool  intermediate));
nfunc(NArray *, NCopySubItems , (NString *path, bool *outSuccess));
nfunc(bool    , NPathExists   , (NString *path, bool *outIsDirectory));
nfunc(void    , NRemovePath   , (NString *path));
