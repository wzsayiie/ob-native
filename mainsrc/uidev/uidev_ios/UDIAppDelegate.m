#import "UDIAppDelegate.h"
#import "UDIViewController.h"

@interface UDIAppDelegate ()
@end

@implementation UDIAppDelegate

- (BOOL) /* ====== */ application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary<UIApplicationLaunchOptionsKey, id> *)options
{
    self.window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    self.window.rootViewController = [[UDIViewController alloc] init];
    [self.window makeKeyAndVisible];

    return YES;
}

@end
