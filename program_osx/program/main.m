#import <Cocoa/Cocoa.h>

int main(int argc, const char *argv[]) {
    
    Class clazz = NSClassFromString(@"UDXAppDelegate");
    id<NSApplicationDelegate> delegate = [[clazz alloc] init];
    //NOTE: the property is weak.
    NSApplication.sharedApplication.delegate = delegate;
    
    return NSApplicationMain(argc, argv);
}
