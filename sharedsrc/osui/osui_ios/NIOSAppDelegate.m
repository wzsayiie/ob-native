#import "NIOSAppDelegate.h"
#import "NIOSViewController.h"
#import "osui.h"

@interface NIOSAppDelegate ()
@end

@implementation NIOSAppDelegate

- (BOOL) /* ====== */ application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary<UIApplicationLaunchOptionsKey, id> *)options
{
    NLaunch();
    
    self.window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    self.window.rootViewController = [[NIOSViewController alloc] init];
    [self.window makeKeyAndVisible];

    return YES;
}

@end
