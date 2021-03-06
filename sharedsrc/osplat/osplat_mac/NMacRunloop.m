#import "nrunloop.h"
#import <Foundation/Foundation.h>

void NPostOnMain(NAction *action) {
    if (!action) {
        return;
    }
    
    NRetain(action);
    dispatch_async(dispatch_get_main_queue(), ^{
        NActionRun(action, NULL);
        NRelease(action);
    });
}
