#import "nrunloop.h"
#import <Foundation/Foundation.h>

void NPostOnMain(NLambda *lambda) {
    if (!lambda) {
        return;
    }
    
    NRetain(lambda);
    dispatch_async(dispatch_get_main_queue(), ^{
        NLambdaPrepareCall(lambda);
        NLambdaCallVoid(lambda);

        NRelease(lambda);
    });
}
