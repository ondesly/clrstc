//
// clrstc micro engine
// сopyright (C) 2019-2020 dmitrii torkhov <dmitriitorkhov@gmail.com>
//

#include <vector>

#include "core/engine.h"

#import "base_view_controller.h"

@interface base_view_controller ()

@property(strong, nonatomic) EAGLContext *context;

@end

@implementation base_view_controller

@synthesize context = _context;

- (void)viewDidLoad {
    [super viewDidLoad];

    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    GLKView *view = (GLKView *) self.view;
    view.context = self.context;

    view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.drawableStencilFormat = GLKViewDrawableStencilFormat8;
    self.preferredFramesPerSecond = 60;

    [EAGLContext setCurrentContext:self.context];
    [((GLKView *) self.view) bindDrawable];

    //

    auto w = float(self.view.bounds.size.width * self.view.contentScaleFactor);
    auto h = float(self.view.bounds.size.height * self.view.contentScaleFactor);

    cc::engine::i()->set_screen_size({w, h});

    if (!cc::engine::i()->is_running()) {
        cc::engine::i()->run();
    }
}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id <UIViewControllerTransitionCoordinator>)coordinator {
    [super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];

    auto w = float(size.width * self.view.contentScaleFactor);
    auto h = float(size.height * self.view.contentScaleFactor);

    cc::engine::i()->set_screen_size({w, h});
}

- (void)dealloc {
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }

    cc::engine::destroy_instance();

    [super dealloc];
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    cc::engine::i()->tick();
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

@end
