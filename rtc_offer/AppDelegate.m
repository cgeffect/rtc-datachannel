#import "AppDelegate.h"
#import "ViewController.h"

@interface AppDelegate ()<NSWindowDelegate>

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    NSRect frame = NSMakeRect(0, 0, 1000, 600);
    self.window = [[NSWindow alloc] initWithContentRect:frame
                                              styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
                                                backing:NSBackingStoreBuffered
                                                  defer:NO
                                                 screen:nil];
    [self.window setTitle:@"Offer"];
    self.window.delegate = self;

    self.viewController = [[ViewController alloc] initWithNibName:nil bundle:nil];
    [self.window setContentView:self.viewController.view];

    [self.window makeKeyAndOrderFront:nil];
    [self.window setIsVisible:YES];
    NSApplication *pApp = (NSApplication *)(aNotification.object);
    [pApp activate];
    
    // 设置窗口的最小和最大大小
    [self.window setContentMinSize:NSMakeSize(1000, 600)];
    [self.window setContentMaxSize:NSMakeSize(1000, 600)];
        
    
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return true;
}

- (BOOL)windowShouldClose:(NSWindow *)sender {
    return true;
}

- (void)windowDidResize:(NSNotification *)notification {
    
}

- (void)windowDidBecomeKey:(NSNotification *)notification {
//    NSLog(@"Window has become key.");
    // 可以在这里执行一些当窗口成为关键窗口时的操作，比如更新界面状态等
}
@end

