#import "nthread.h"
#import <Foundation/Foundation.h>

void NRunThread(NLambda *lambda) {
    if (!lambda) {
        return;
    }
    
    NRetain(lambda);
    NSThread *thread = [[NSThread alloc] initWithBlock:^{
        NLambdaPrepareCall(lambda);
        NLambdaCallVoid(lambda);

        NRelease(lambda);
    }];
    [thread start];
}

void NThreadSleep(float seconds) {
    if (seconds > 0) {
        [NSThread sleepForTimeInterval:seconds];
    }
}
