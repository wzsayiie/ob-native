#import "UDWindowController.h"
#import "UDViewController.h"

@interface UDWindowController () <NSWindowDelegate>
@end

@implementation UDWindowController

+ (UDWindowController *)windowController {
    //onyl assign the origin, the size will be taken care of by contentView.
    NSRect rect = NSMakeRect(100, 500, 0, 0);
    
    NSWindowStyleMask style = 0;
    style |= NSWindowStyleMaskTitled;
    style |= NSWindowStyleMaskClosable;
    style |= NSWindowStyleMaskMiniaturizable;
    style |= NSWindowStyleMaskResizable;
    
    NSBackingStoreType backing = NSBackingStoreBuffered;
    
    NSWindow *window = [[NSWindow alloc] initWithContentRect:rect styleMask:style backing:backing defer:NO];
    UDWindowController *controller = [[UDWindowController alloc] initWithWindow:window];
    window.contentViewController = [[UDViewController alloc] init];
    window.delegate = controller;
    
    return controller;
}

@end
