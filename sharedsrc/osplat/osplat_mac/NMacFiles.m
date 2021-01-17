#import "nfiles.h"
#import <Foundation/Foundation.h>

static NSString *_NS(NString *string) {
    const char *chars = NStringU8Chars(string);
    return [NSString stringWithUTF8String:chars];
}

static NString *_NCreateS(NSString *string) {
    return NStringCreateWithUTFChars(NUTF8, string.UTF8String);
}

NString *NCopyDocumentPath(void) {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    return _NCreateS(paths.firstObject);
}

NString *NCopyCachesPath(void) {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    return _NCreateS(paths.firstObject);
}

NString *NCopyTemporaryPath(void) {
    NSString *path = NSTemporaryDirectory();
    return _NCreateS(path);
}

bool NMakeDirectory(NString *path, bool intermediate) {
    if (!NStringIsEmpty(path)) {
        NSFileManager *manager = NSFileManager.defaultManager;
        return [manager createDirectoryAtPath:_NS(path) withIntermediateDirectories:intermediate attributes:nil error:nil];
    }
    return false;
}

NArray *NCopySubitems(NString *path, bool *success) {
    if (NStringIsEmpty(path)) {
        if (success) {
            *success = false;
        }
        return NULL;
    }
    
    //traverse the directory.
    NSFileManager *manager = NSFileManager.defaultManager;
    NSError *error = nil;
    NSArray<NSString *> *contents = [manager contentsOfDirectoryAtPath:_NS(path) error:&error];
    if (error) {
        if (success) {
            *success = false;
        }
        return NULL;
    }

    //NOTE: need to sort.
    contents = [contents sortedArrayUsingComparator:^NSComparisonResult(NSString *a, NSString *b) {
        return [a compare:b];
    }];

    //construct returned list.
    NArray *subitems = NArrayCreate();
    for (NSString *entry in contents) {
        NString *item = _NCreateS(entry);
        NArrayPush(subitems, item);
        NRelease(item);
    }
    
    if (success) {
        *success = true;
    }
    return subitems;
}

bool NPathExists(NString *path, bool *isDirectory) {
    BOOL directory = NO;
    bool exists = false;

    if (!NStringIsEmpty(path)) {
        NSFileManager *manager = NSFileManager.defaultManager;
        return [manager fileExistsAtPath:_NS(path) isDirectory:&directory];
    }
    
    if (isDirectory) {
        *isDirectory = directory;
    }
    return exists;
}

void NRemovePath(NString *path) {
    if (!NStringIsEmpty(path)) {
        NSFileManager *manager = NSFileManager.defaultManager;
        [manager removeItemAtPath:_NS(path) error:nil];
    }
}
