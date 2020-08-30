#import "UDAppDelegate.h"
#import "UDWindowController.h"

@interface UDAppDelegate ()
@property (nonatomic) UDWindowController *windowController;
@end

@implementation UDAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    self.windowController = [UDWindowController windowController];
    [self.windowController showWindow:nil];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)app {
    return YES;
}

@end
