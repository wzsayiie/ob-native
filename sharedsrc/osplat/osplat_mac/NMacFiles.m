#import "nfiles.h"
#import <Foundation/Foundation.h>

static NSString *NS(NString *string) {
    const char *chars = NStringU8Chars(string);
    return [NSString stringWithUTF8String:chars];
}

static NString *CreateS(NSString *string) {
    return NStringCreateWithUTFChars(NUTF8, string.UTF8String);
}

NString *NCopyDocumentPath(void) {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    return CreateS(paths.firstObject);
}

NString *NCopyCachesPath(void) {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    return CreateS(paths.firstObject);
}

NString *NCopyTemporaryPath(void) {
    NSString *path = NSTemporaryDirectory();
    return CreateS(path);
}

bool NMakeDirectory(NString *path, bool intermediate) {
    if (!NStringIsEmpty(path)) {
        NSFileManager *manager = NSFileManager.defaultManager;
        return [manager createDirectoryAtPath:NS(path) withIntermediateDirectories:intermediate attributes:nil error:nil];
    }
    return false;
}

static void SetOutBool(bool *output, bool value) {
    if (output) {
        *output = value;
    }
}

NArray *NCopySubItems(NString *path, bool *outSuccess) {
    if (NStringIsEmpty(path)) {
        SetOutBool(outSuccess, false);
        return NULL;
    }
    
    //traverse the directory.
    NSFileManager *manager = NSFileManager.defaultManager;
    NSError *error = nil;
    NSArray<NSString *> *contents = [manager contentsOfDirectoryAtPath:NS(path) error:&error];
    if (error) {
        SetOutBool(outSuccess, false);
        return NULL;
    }

    //NOTE: need to sort.
    contents = [contents sortedArrayUsingComparator:^NSComparisonResult(NSString *a, NSString *b) {
        return [a compare:b];
    }];

    //construct returned list.
    NArray *subitems = NArrayCreate();
    for (NSString *entry in contents) {
        NString *item = CreateS(entry);
        NArrayPush(subitems, item);
        NRelease(item);
    }
    
    SetOutBool(outSuccess, true);
    return subitems;
}

bool NPathExists(NString *path, bool *outIsDirectory) {
    BOOL directory = NO;
    bool exists = false;

    if (!NStringIsEmpty(path)) {
        NSFileManager *manager = NSFileManager.defaultManager;
        return [manager fileExistsAtPath:NS(path) isDirectory:&directory];
    }
    
    SetOutBool(outIsDirectory, directory);
    return exists;
}

void NRemovePath(NString *path) {
    if (!NStringIsEmpty(path)) {
        NSFileManager *manager = NSFileManager.defaultManager;
        [manager removeItemAtPath:NS(path) error:nil];
    }
}
