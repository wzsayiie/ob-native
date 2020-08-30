#import "UDIAppDelegate.h"
#import "UDIViewController.h"
#import "uidev.h"

@interface UDIAppDelegate ()
@end

@implementation UDIAppDelegate

- (BOOL) /* ====== */ application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary<UIApplicationLaunchOptionsKey, id> *)options
{
    appmain();
    
    self.window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    self.window.rootViewController = [[UDIViewController alloc] init];
    [self.window makeKeyAndVisible];

    return YES;
}

@end
