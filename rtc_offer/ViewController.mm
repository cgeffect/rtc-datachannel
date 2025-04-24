#import "ViewController.h"
#include "rtc/offerer.h"
#include <string>

@interface ViewController ()
{
    NSTextView *localLable;
    NSTextView *localSdpView;
    NSTextView *candidateView;
    NSTextView *localMessageView;
    NSButton *button;

    NSTextView *remoteLable;
    NSTextView *remoteSdpView;
    NSTextView *remoteCandidateView;
    NSTextView *remoteMessageView;
    NSButton *sdpBtn;
    NSButton *candidateBtn;
    offerer *offer_;
}
@end

@implementation ViewController

- (instancetype)init {
    self = [super init];
    if (self) {
        NSView *view = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 1000, 600)];
        view.wantsLayer = YES;
        view.layer.backgroundColor = [NSColor whiteColor].CGColor;
        self.view = view;
    }
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self setupLocalUI];
    [self setupRemoteUI];
    
    offer_ = new offerer();
    offer_->setSdpCallback([=](std::string sdp) {
        dispatch_async(dispatch_get_main_queue(), ^{
            std::string sdp_str = sdp;
            NSString *sdp_ns = [NSString stringWithUTF8String:sdp_str.c_str()];
            localSdpView.string = sdp_ns;
        });
    });
    
    offer_->setCandidateCallback([=](std::string candidate) {
        dispatch_async(dispatch_get_main_queue(), ^{
            std::string candidate_str = candidate;
            NSString *candidate_ns = [NSString stringWithUTF8String:candidate_str.c_str()];
            NSString *message = candidateView.string;
            if (message.length == 0) {
                message = candidate_ns;
            } else {
                message = [NSString stringWithFormat:@"%@\n%@", candidateView.string, candidate_ns];
            }
            candidateView.string = message;
        });
    });
    offer_->startOffer();
}

- (void)setupLocalUI {
    
    // 创建 NSTextView 作为标签
    localLable = [[NSTextView alloc] initWithFrame:NSMakeRect(0, 600 - 30, 450, 30)];
    [localLable setString:@"local description"];
    [localLable setEditable:NO];
    [localLable setSelectable:NO];
    [localLable setDrawsBackground:NO];
    localLable.wantsLayer = YES;
    localLable.layer.backgroundColor = [NSColor redColor].CGColor;
    [self.view addSubview:localLable];
    
    NSScrollView *scrollView = [[NSScrollView alloc] initWithFrame:NSMakeRect(0, 600 - 330, 450, 300)];
    scrollView.hasVerticalScroller = YES; // 启用垂直滚动条
    scrollView.hasHorizontalScroller = NO; // 禁用水平滚动条
    [scrollView setDrawsBackground:NO];
    scrollView.wantsLayer = YES;
    scrollView.layer.backgroundColor = [NSColor yellowColor].CGColor;
    [self.view addSubview:scrollView];
    
    localSdpView = [[NSTextView alloc] initWithFrame:NSMakeRect(0, 600 - 330, 450, 300)];
    localSdpView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
    localSdpView.layer.backgroundColor = [NSColor redColor].CGColor;
    [scrollView setDocumentView:localSdpView];
    
    candidateView = [[NSTextView alloc] initWithFrame:NSMakeRect(0, 600 - 370, 450, 30)];
    [candidateView setEditable:NO];
    [candidateView setDrawsBackground:NO];
    candidateView.wantsLayer = YES;
    candidateView.layer.backgroundColor = [NSColor yellowColor].CGColor;
    [self.view addSubview:candidateView];
    
    localMessageView = [[NSTextView alloc] initWithFrame:NSMakeRect(0, 600 - 480, 450, 100)];
    [self.view addSubview:localMessageView];
    
    button = [NSButton buttonWithTitle:@"send" target:self action:@selector(sendClicked:)];
    [button setBezelStyle:NSRoundedBezelStyle];
    [button setFrame:NSMakeRect(0, 600 - 510, 80, 30)];
    [self.view addSubview:button];
}

- (void)setupRemoteUI {
    float offsetX = 550;
    // 创建 NSTextView 作为标签
    remoteLable = [[NSTextView alloc] initWithFrame:NSMakeRect(offsetX, 600 - 30, 450, 30)];
    [remoteLable setString:@"remote description"];
    [remoteLable setEditable:NO];
    [remoteLable setSelectable:NO];
    [remoteLable setDrawsBackground:NO];
    remoteLable.wantsLayer = YES;
    remoteLable.layer.backgroundColor = [NSColor redColor].CGColor;
    [self.view addSubview:remoteLable];
    
    NSScrollView *scrollView = [[NSScrollView alloc] initWithFrame:NSMakeRect(offsetX, 600 - 330, 450, 300)];
    scrollView.hasVerticalScroller = YES; // 启用垂直滚动条
    scrollView.hasHorizontalScroller = NO; // 禁用水平滚动条
    [scrollView setDrawsBackground:NO];
    scrollView.wantsLayer = YES;
    scrollView.layer.backgroundColor = [NSColor yellowColor].CGColor;
    [self.view addSubview:scrollView];
    
    remoteSdpView = [[NSTextView alloc] initWithFrame:NSMakeRect(offsetX, 600 - 330, 450, 300)];
    remoteSdpView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
    remoteSdpView.layer.backgroundColor = [NSColor redColor].CGColor;
    [scrollView setDocumentView:remoteSdpView];
    
    remoteCandidateView = [[NSTextView alloc] initWithFrame:NSMakeRect(offsetX, 600 - 370, 450, 30)];
    [remoteCandidateView setDrawsBackground:NO];
    remoteCandidateView.wantsLayer = YES;
    remoteCandidateView.layer.backgroundColor = [NSColor yellowColor].CGColor;
    [self.view addSubview:remoteCandidateView];
    
    remoteMessageView = [[NSTextView alloc] initWithFrame:NSMakeRect(offsetX, 600 - 480, 450, 100)];
    [self.view addSubview:remoteMessageView];
    
    sdpBtn = [NSButton buttonWithTitle:@"sdp" target:self action:@selector(sdpClicked:)];
    [sdpBtn setBezelStyle:NSRoundedBezelStyle];
    [sdpBtn setFrame:NSMakeRect(offsetX, 600 - 510, 80, 30)];
    [self.view addSubview:sdpBtn];

    candidateBtn = [NSButton buttonWithTitle:@"candicate" target:self action:@selector(candicateClicked:)];
    [candidateBtn setBezelStyle:NSRoundedBezelStyle];
    [candidateBtn setFrame:NSMakeRect(offsetX + 100, 600 - 510, 80, 30)];
    [self.view addSubview:candidateBtn];

}

- (void)sendClicked:(id)sender {
    offer_->printConnectionInfo();

    std::string msg = [localMessageView.string UTF8String];
    offer_->setMessage(msg);
}

- (void)sdpClicked:(id)sender {
    if (remoteSdpView.string.length == 0) {
        NSLog(@"not set remote sdp!!!");
        return;;
    }
    
    offer_->setRemoteSdp([remoteSdpView.string UTF8String]);   
}

- (void)candicateClicked:(id)sender {
    if (remoteCandidateView.string.length == 0) {
        NSLog(@"not set candidate sdp!!!");
        return;;
    }
    offer_->setRemoteCandidate([remoteCandidateView.string UTF8String]);
}

@end
