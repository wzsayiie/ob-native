#import "nthread.h"
#import <Foundation/Foundation.h>

void NRunThread(NAction *action) {
    if (!action) {
        return;
    }
    
    NRetain(action);
    NSThread *thread = [[NSThread alloc] initWithBlock:^{
        NActionRun(action, NULL);
        NRelease(action);
    }];
    [thread start];
}

void NThreadSleep(float seconds) {
    if (seconds > 0) {
        [NSThread sleepForTimeInterval:seconds];
    }
}
