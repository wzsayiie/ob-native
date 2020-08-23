#import "UDAppDelegate.h"
#import "UDViewController.h"

@interface UDAppDelegate ()
@end

@implementation UDAppDelegate

- (BOOL) /* ====== */ application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary<UIApplicationLaunchOptionsKey, id> *)options
{
    self.window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    self.window.rootViewController = [[UDViewController alloc] init];
    [self.window makeKeyAndVisible];

    return YES;
}

@end
