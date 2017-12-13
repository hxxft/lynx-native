// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LYXCoordinatorActionExecutor.h"
#import "LYXPixelUtil.h"
#import "LYXDefines.h"

@implementation LYXCoordinatorActionExecutor {
    LynxUI *_ui;
}

LYX_NOT_IMPLEMENTED(- (instancetype) init)

- (instancetype) initWithUI:(LynxUI *) ui {
    self = [super init];
    if (self) {
        _ui = ui;
    }
    return self;
}

- (void) postEvent:(lynx::CoordinatorAction) action {
    if (!action.event_.empty()) {
        NSString *eventName = [[[NSString alloc] initWithUTF8String:action.event_.c_str()] lowercaseString];
        id params = nil;
        switch(action.params_for_event_.type_) {
            case lepus::Value_Number:
                params = [[NSNumber alloc] initWithDouble: action.params_for_event_.number_];
                break;
            case lepus::Value_String:
                params = [[NSString alloc] initWithUTF8String: action.params_for_event_.str_->c_str()];
                break;
            default: break;
        }
        NSDictionary *event = [[NSDictionary alloc] initWithObjects:@[params] forKeys:@[@"detail"]];
        [_ui postEvent:eventName withValue:[[NSArray alloc] initWithObjects:event, nil]];
    }
}

- (void) executeAction:(lynx::CoordinatorAction) action {
    
    [UIView animateWithDuration:action.duration_ != lynx::CoordinatorAction::kNotSet ? action.duration_ / 1000 : 0
                          delay:0
                        options:UIViewAnimationOptionPreferredFramesPerSecond60
                     animations:^{

                         BOOL isTransformChanged = NO;
                         UIView *view = _ui.view;
                         CATransform3D transform = CATransform3DIdentity;

                         if (action.translate_x_ != lynx::CoordinatorAction::kNotSet) {
                             transform = CATransform3DTranslate(transform, [LYXPixelUtil lynxNumberToPx:action.translate_x_], 0, 0);
                             isTransformChanged = YES;
                         }
                         if (action.translate_y_ != lynx::CoordinatorAction::kNotSet) {
                             transform = CATransform3DTranslate(transform, 0, [LYXPixelUtil lynxNumberToPx:action.translate_y_], 0);
                             isTransformChanged = YES;
                         }
                         if (action.scale_x_ != lynx::CoordinatorAction::kNotSet) {
                             // 1.001 is a black magic for fix animation bug that when changed translateX and scaleX,
                             // the view will jump to the wrong position and start animation.
                             // Reffer to: https://stackoverflow.com/a/33465048/5167293
                             transform = CATransform3DScale(transform, action.scale_x_, 1.0, 1.001);
                             isTransformChanged = YES;
                         }
                         if (action.scale_y_ != lynx::CoordinatorAction::kNotSet) {
                             transform = CATransform3DScale(transform, 1.0, action.scale_y_, 1.001);
                             isTransformChanged = YES;
                         }
                         if (action.rotate_x_ != lynx::CoordinatorAction::kNotSet) {
                             CGFloat angle = action.rotate_x_ / 360.0 * 2 * M_PI;
                             transform = CATransform3DRotate(transform, angle, 1, 0, 0);
                             isTransformChanged = YES;
                         }
                         if (action.rotate_y_ != lynx::CoordinatorAction::kNotSet) {
                             CGFloat angle = action.rotate_y_ / 360.0 * 2 * M_PI;
                             transform = CATransform3DRotate(transform, angle, 0, 1, 0);
                             isTransformChanged = YES;
                         }
                         if (action.alpha_ != lynx::CoordinatorAction::kNotSet){
                             view.alpha = action.alpha_;
                         }
                         CGPoint anchorPoint = view.layer.anchorPoint;
                         if (action.pivot_x_ != lynx::CoordinatorAction::kNotSet) {
                             anchorPoint.x = [LYXPixelUtil lynxNumberToPx:action.pivot_x_] / view.frame.size.width;
                             isTransformChanged = YES;
                         }
                         if (action.pivot_y_ != lynx::CoordinatorAction::kNotSet) {
                             anchorPoint.y = [LYXPixelUtil lynxNumberToPx:action.pivot_y_] / view.frame.size.height;
                             isTransformChanged = YES;
                         }

                         BOOL isLayoutOffsetChanged = NO;
                         if (action.offset_top_ != lynx::CoordinatorAction::kNotSet) {
                             [_ui setOffsetTop:[LYXPixelUtil lynxNumberToPx:action.offset_top_]];
                             isLayoutOffsetChanged = YES;
                         }
                         if (action.offset_bottom_ != lynx::CoordinatorAction::kNotSet) {
                             [_ui setOffsetBottom:[LYXPixelUtil lynxNumberToPx:action.offset_bottom_]];
                             isLayoutOffsetChanged = YES;
                         }
                         if (action.offset_left_ != lynx::CoordinatorAction::kNotSet) {
                             [_ui setOffsetLeft:[LYXPixelUtil lynxNumberToPx:action.offset_left_]];
                             isLayoutOffsetChanged = YES;
                         }
                         if (action.offset_right_ != lynx::CoordinatorAction::kNotSet) {
                             [_ui setOffsetRight:[LYXPixelUtil lynxNumberToPx:action.offset_right_]];
                             isLayoutOffsetChanged = YES;
                         }

                         if (isLayoutOffsetChanged) {
                             [_ui updateBounds];
                         }
                         if (isTransformChanged) {
                             view.layer.transform = transform;
                             [self setAnchorPoint:anchorPoint forView:view];
                         }
                     } completion:^(BOOL finished){
                         [self postEvent:action];
                     }
     ];
}

- (void)setAnchorPoint:(CGPoint)anchorPoint forView:(UIView *)view
{
    CGPoint newPoint = CGPointMake(view.bounds.size.width * anchorPoint.x,
                                   view.bounds.size.height * anchorPoint.y);
    CGPoint oldPoint = CGPointMake(view.bounds.size.width * view.layer.anchorPoint.x,
                                   view.bounds.size.height * view.layer.anchorPoint.y);
    
    newPoint = CGPointApplyAffineTransform(newPoint, view.transform);
    oldPoint = CGPointApplyAffineTransform(oldPoint, view.transform);
    
    CGPoint position = view.layer.position;
    
    position.x -= oldPoint.x;
    position.x += newPoint.x;
    
    position.y -= oldPoint.y;
    position.y += newPoint.y;
    
    view.layer.position = position;
    view.layer.anchorPoint = anchorPoint;
}

@end
