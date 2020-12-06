#import "NOSXAppDelegate.h"
#import "NOSXWindowController.h"
#import "uidev.h"

@interface NOSXAppDelegate ()
@property (nonatomic) NOSXWindowController *windowController;
@end

@implementation NOSXAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    NLaunch();
    
    self.windowController = [NOSXWindowController windowController];
    [self.windowController showWindow:nil];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)app {
    return YES;
}

@end
