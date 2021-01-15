#include "nfiles.h"

NString *NCopyDocumentPath(void) {
    return NULL;
}

NString *NCopyCachesPath(void) {
    return NULL;
}

NString *NCopyTemporaryPath(void) {
    return NULL;
}

bool NMakeDirectory(NString *path, bool intermediate) {
    return false;
}

NArray *NCopySubitems(NString *path, bool *success) {
    return NULL;
}

bool NPathExists(NString *path, bool *isDirectory) {
    return false;
}

void NRemovePath(NString *path) {
}
