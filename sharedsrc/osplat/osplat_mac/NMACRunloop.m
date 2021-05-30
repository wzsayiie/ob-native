#import "nrunloop.h"
#import <Foundation/Foundation.h>

void NPostTaskOnMain(int64_t param, NTaskFunc func) {
    if (!func) {
        return;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        func(param);
    });
}
