#include "nfiles.h"

NString *NCopyDocumentPath(void) {
    return NULL;
}

NString *NCopyCachesPath(void) {
    return NULL;
}

NString *NCopyTemporaryPath(void) {
    return NCopyCachesPath();
}

bool NMakeDirectory(NString *path, bool intermediate) {
    return false;
}

NArray *NCopySubItems(NString *path, bool *outSuccess) {
    return NULL;
}

bool NPathExists(NString *path, bool *outIsDirectory) {
    return false;
}

void NRemovePath(NString *path) {
}
