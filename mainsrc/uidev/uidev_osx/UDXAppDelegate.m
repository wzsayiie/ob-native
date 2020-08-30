#import "UDXAppDelegate.h"
#import "UDXWindowController.h"

@interface UDXAppDelegate ()
@property (nonatomic) UDXWindowController *windowController;
@end

@implementation UDXAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    self.windowController = [UDXWindowController windowController];
    [self.windowController showWindow:nil];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)app {
    return YES;
}

@end
