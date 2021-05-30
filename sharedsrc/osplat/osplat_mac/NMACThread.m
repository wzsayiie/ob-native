#import "nthread.h"
#import <Foundation/Foundation.h>

void NRunThread(int64_t param, NThreadFunc func) {
    if (!func) {
        return;
    }
    
    NSThread *thread = [[NSThread alloc] initWithBlock:^{
        func(param);
    }];
    [thread start];
}

void NThreadSleep(float seconds) {
    if (seconds > 0) {
        [NSThread sleepForTimeInterval:seconds];
    }
}
