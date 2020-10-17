#import "NMAppDelegate.h"
#import "NMWindowController.h"
#import "uidev.h"

@interface NMAppDelegate ()
@property (nonatomic) NMWindowController *windowController;
@end

@implementation NMAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    NMain();
    
    self.windowController = [NMWindowController windowController];
    [self.windowController showWindow:nil];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)app {
    return YES;
}

@end
