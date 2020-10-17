#import "NIAppDelegate.h"
#import "NIViewController.h"
#import "uidev.h"

@interface NIAppDelegate ()
@end

@implementation NIAppDelegate

- (BOOL) /* ====== */ application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary<UIApplicationLaunchOptionsKey, id> *)options
{
    NMain();
    
    self.window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    self.window.rootViewController = [[NIViewController alloc] init];
    [self.window makeKeyAndVisible];

    return YES;
}

@end
