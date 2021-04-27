#include "nfiles.h"
#include "osbind.h"

static NJNIClass *FilesClass  (void) {return NJNIImportClass("src/shared/osplat/NFiles");}
static NJNIClass *BooleanClass(void) {return NJNIImportClass("boolean");}
static NJNIClass *StringClass (void) {return NJNIImportClass("java/lang/String");}

static NJNIClass *BooleanArrayClass(void) {return NJNIImportArrayClass(BooleanClass());}
static NJNIClass *StringArrayClass (void) {return NJNIImportArrayClass(StringClass ());}

NString *NCopyDocumentPath(void) {
    static NJNIMethod *method = NULL;
    if (!method) {
        NJNIFinderPrepare();
        NJNIFinderSetRet(StringClass());
        method = NJNIFindStaticMethod(FilesClass(), "documentPath");
    }

    NJNICallerPrepareCall();
    return NJNICallRetainString(NULL, method);
}

NString *NCopyCachesPath(void) {
    static NJNIMethod *method = NULL;
    if (!method) {
        NJNIFinderPrepare();
        NJNIFinderSetRet(StringClass());
        method = NJNIFindStaticMethod(FilesClass(), "cachesPath");
    }

    NJNICallerPrepareCall();
    return NJNICallRetainString(NULL, method);
}

NString *NCopyTemporaryPath(void) {
    return NCopyCachesPath();
}

bool NMakeDirectory(NString *path, bool intermediate) {
    static NJNIMethod *method = NULL;
    if (!method) {
        NJNIFinderPrepare();
        NJNIFinderSetRet(BooleanClass());
        NJNIFinderAddArg(StringClass ());
        NJNIFinderAddArg(BooleanClass());
        method = NJNIFindStaticMethod(FilesClass(), "makeDirectory");
    }

    NJNICallerPrepareCall();
    NJNICallerPushString(path);
    NJNICallerPushBoolean(intermediate);
    return NJNICallBoolean(NULL, method);
}

static void SetOutBool(bool *out, NJNIObject *value) {
    if (out) {
        *out = NJNIBooleanItem(value, 0);
    }
}

static NArray *CopyJNIStringArray(NJNIObject *object) {
    int count = NJNIArrayCount(object);

    NArray *array = NArrayCreate();
    for (int idx = 0; idx < count; ++idx) {
        NString *item = NJNIRetainStringItem(object, idx);
        NArrayPush(array, item);
        NRelease(item);
    }
    return array;
}

NArray *NCopySubItems(NString *path, bool *outSuccess) {
    static NJNIMethod *method = NULL;
    if (!method) {
        NJNIFinderPrepare();
        NJNIFinderSetRet(StringArrayClass ());
        NJNIFinderAddArg(StringClass      ());
        NJNIFinderAddArg(BooleanArrayClass());
        method = NJNIFindStaticMethod(FilesClass(), "copySubItems");
    }

    NJNIObject *outSuccessObject = NJNICreateArray(BooleanArrayClass(), 1);
    NJNIObject *subItemsObject = NULL;
    NArray *subItems = NULL;
    {
        NJNICallerPrepareCall();
        NJNICallerPushString(path);
        NJNICallerPushObject(outSuccessObject);

        subItemsObject = NJNICallRetainObject(NULL, method);
        subItems = CopyJNIStringArray(subItemsObject);
        SetOutBool(outSuccess, outSuccessObject);
    }
    NRelease(outSuccessObject);
    NRelease(subItemsObject);

    return subItems;
}

bool NPathExists(NString *path, bool *outIsDirectory) {
    static NJNIMethod *method = NULL;
    if (!method) {
        NJNIFinderPrepare();
        NJNIFinderSetRet(BooleanClass     ());
        NJNIFinderAddArg(StringClass      ());
        NJNIFinderAddArg(BooleanArrayClass());
        method = NJNIFindStaticMethod(FilesClass(), "pathExists");
    }

    NJNIObject *outIsDirectoryObject = NJNICreateArray(BooleanArrayClass(), 1);
    bool exists = false;
    {
        NJNICallerPrepareCall();
        NJNICallerPushString(path);
        NJNICallerPushObject(outIsDirectoryObject);

        exists = NJNICallBoolean(NULL, method);
        SetOutBool(outIsDirectory, outIsDirectoryObject);
    }
    NRelease(outIsDirectoryObject);

    return exists;
}

void NRemovePath(NString *path) {
    static NJNIMethod *method = NULL;
    if (!method) {
        NJNIFinderPrepare();
        NJNIFinderAddArg(StringClass());
        method = NJNIFindStaticMethod(FilesClass(), "removePath");
    }

    NJNICallerPrepareCall();
    NJNICallerPushString(path);
    NJNICallVoid(NULL, method);
}
