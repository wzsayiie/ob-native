#import "NOSXWindowController.h"
#import "NOSXViewController.h"

@interface NOSXWindowController () <NSWindowDelegate>
@end

@implementation NOSXWindowController

+ (NOSXWindowController *)windowController {
    //onyl assign the origin, the size will be taken care of by contentView.
    NSRect rect = NSMakeRect(100, 500, 0, 0);
    
    NSWindowStyleMask style = 0;
    style |= NSWindowStyleMaskTitled;
    style |= NSWindowStyleMaskClosable;
    style |= NSWindowStyleMaskMiniaturizable;
    style |= NSWindowStyleMaskResizable;
    
    NSBackingStoreType backing = NSBackingStoreBuffered;
    
    NSWindow *window = [[NSWindow alloc] initWithContentRect:rect styleMask:style backing:backing defer:NO];
    NOSXWindowController *controller = [[NOSXWindowController alloc] initWithWindow:window];
    window.contentViewController = [[NOSXViewController alloc] init];
    window.delegate = controller;
    
    return controller;
}

@end
