#import "UDXWindowController.h"
#import "UDXViewController.h"

@interface UDXWindowController () <NSWindowDelegate>
@end

@implementation UDXWindowController

+ (UDXWindowController *)windowController {
    //onyl assign the origin, the size will be taken care of by contentView.
    NSRect rect = NSMakeRect(100, 500, 0, 0);
    
    NSWindowStyleMask style = 0;
    style |= NSWindowStyleMaskTitled;
    style |= NSWindowStyleMaskClosable;
    style |= NSWindowStyleMaskMiniaturizable;
    style |= NSWindowStyleMaskResizable;
    
    NSBackingStoreType backing = NSBackingStoreBuffered;
    
    NSWindow *window = [[NSWindow alloc] initWithContentRect:rect styleMask:style backing:backing defer:NO];
    UDXWindowController *controller = [[UDXWindowController alloc] initWithWindow:window];
    window.contentViewController = [[UDXViewController alloc] init];
    window.delegate = controller;
    
    return controller;
}

@end
