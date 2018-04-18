// Copyright 2017 The Lynx Authors. All rights reserved.

#include "widget/lynx_ui.h"

#include "base/ios/common.h"

#include "base/defines.h"
#include "base/ui_event_action.h"
#include "base/ui_update_data_action.h"

#include "render/render_object_type.h"
#include "render/ios/render_object_impl_ios.h"

#include "widget/body/lynx_ui_body.h"
#include "widget/coordinator/coordinator_treatment.h"
#include "widget/coordinator/coordinator_action_executor.h"
#include "widget/coordinator/coordinator_types.h"

static NSString * const kAttrCoordinatorAffinity = @"coordinator-affinity";
static NSString * const kAttrCoordinatorTag = @"coordinator-tag";
static NSString * const kAttrCoordinatorType = @"coordinator-type";


@interface LynxUI()
@property(nonatomic) LxCrdTypes *coordinatorTypes;
@end

@implementation LynxUI

#pragma mark - LynxUI

LX_NOT_IMPLEMENTED(- (instancetype)init)

- (id)initWithRenderObjectImpl:(RenderObjectImplBridge *) impl {
    self = [super init];
    if (self) {
        _view = [self createView:impl];
        _coordinatorTreatment = [[LxCrdTreatment alloc] initWithResponder:self actionExecutor:[[LxCrdActionExecutor alloc] initWithUI:self]];
        [self bindRenderObjectImpl:impl];
    }
    return self;
}

- (BOOL) isEmpty {
    return _view == nil;
}

- (UIView *) getView {
    return _view;
}

- (UIView *) createView:(RenderObjectImplBridge *) impl {
    @throw [NSException exceptionWithName:NSInternalInconsistencyException
                                   reason:[NSString stringWithFormat:@"You must override %@ in a subclass", NSStringFromSelector(_cmd)]
                                 userInfo:nil];
}

- (void) updateStyle:(const lynx::CSSStyle&) style {
    if (!self.view) {
        return;
    }
    self.view.backgroundColor = COLOR_CONVERT(style.background_color_);
    self.view.alpha = style.opacity_;
    // Border radius setting
    if (style.border_radius_ > 0) {
        self.view.layer.cornerRadius = style.border_radius_ ;
        self.view.layer.masksToBounds = YES;
    } else {
        self.view.layer.cornerRadius = 0;
    }
    // Border width setting
    if (style.border_width_ > 0) {
        self.view.layer.borderColor = [COLOR_CONVERT(style.border_color_) CGColor];
        self.view.layer.borderWidth = style.border_width_;
    }
}

- (void) setPosition:(const base::Position&) position {
    _renderObjectImpl->position_ = position;
    [self updateFrame];
}

- (void) updateFrame {
    if(_renderObjectImpl->position_.IsEmpty() || !_view) return;
    CGFloat x = _renderObjectImpl->position_.left_;
    CGFloat y = _renderObjectImpl->position_.top_;
    CGFloat width = _renderObjectImpl->position_.right_ + _offsetRight;
    CGFloat height = _renderObjectImpl->position_.bottom_ + _offsetBottom;
    _view.frame = CGRectMake(x, y, width, height);
    [self updateBounds];
}

- (void) updateBounds {
    if(_renderObjectImpl->position_.IsEmpty() || !_view) return;
    CGFloat x = _renderObjectImpl->position_.left_ + _offsetLeft;
    CGFloat y = _renderObjectImpl->position_.top_ + _offsetTop;
    CGFloat width = (_renderObjectImpl->position_.right_ + _offsetRight) - x;
    CGFloat height = (_renderObjectImpl->position_.bottom_ + _offsetBottom) - y;
    _view.center = CGPointMake(x + width / 2, y + height / 2);
    _view.bounds = CGRectMake(0, 0, width, height);
}

- (void) setSize:(const base::Size &) size {
    
}

- (void) insertChild:(RenderObjectImplBridge *) child atIndex:(int) index {
    
}

- (void) removeChild:(RenderObjectImplBridge *) child {
    
}

- (void) setText:(NSString *) text {
    
}

- (void) setAttribute:(NSString *) value forKey:(NSString *) key {
    if ([kAttrCoordinatorTag isEqualToString:key]) {
        [self setCoordinatorTag:value];
    } else if ([kAttrCoordinatorAffinity isEqualToString:key]) {
        [self setCoodinatorAffinity:value];
    } else if ([kAttrCoodinatorCommand isEqualToString:key]) {
        [self setCoordinatorCommands:value];
    } else if ([kAttrCoordinatorType isEqualToString:key]) {
        [self setCoordinatorType:value];
    }
}

- (void) requestLayout {
    
}

- (void) setData:(id) value withKey:(LynxRenderObjectAttr) attr {
    
}

- (void) addEventListener:(NSString *) event {
    if ([event isEqualToString:@"click"]) {
        _view.userInteractionEnabled = YES;
        UITapGestureRecognizer *singleTap = [[UITapGestureRecognizer alloc]
                      initWithTarget:self action:@selector(handleSingleTap:)];
        [_view addGestureRecognizer:singleTap];
    }
}

-(void) handleSingleTap:(id)sender {
    NSMutableArray *array = [[NSMutableArray alloc] init];
    NSMutableDictionary *event = [[NSMutableDictionary alloc] init];
    [event setValue:@"click" forKey:@"type"];
    array[0] = event;
    [self postEvent:@"click" withValue:array];
}

- (void) removeEventListener:(NSString *) event {
    
}

- (void) bindRenderObjectImpl:(RenderObjectImplBridge *) impl {
    _renderObjectImpl = impl;
    [self updateStyle:impl->style_];
    [self setPosition:impl->position_];
    [self setAttribution:impl];
    [self addEvents:impl];
}

- (void) setAttribution:(RenderObjectImplBridge *) impl {
    if (impl.attributes.count != 0) {
        for (id key in impl.attributes) {
            id value = impl.attributes[key];
            [self setAttribute:SAFE_CONVERT(value, NSString)
                        forKey:SAFE_CONVERT(key, NSString)];
        }
    }
}

- (void) addEvents:(RenderObjectImplBridge *) impl {
    if (impl.events.count != 0) {
        for (id key in impl.events) {
            [self addEventListener:SAFE_CONVERT(key, NSString)];
        }
    }
}

- (void) unbindRenderObjectImpl {
    _renderObjectImpl = nil;
}

- (LxUIBody *) getRootUI {
    if (_renderObjectImpl && _renderObjectImpl.root) {
        return (LxUIBody *)_renderObjectImpl.root.ui;
    }
    return nil;
}

- (void) postEvent:(NSString *)event withValue:(NSArray *)array {
    LxUIBody *body = [self getRootUI];
    if (body) {
        [body collectAction:[[LynxUIEventAction alloc] initWithTarget:_renderObjectImpl andEvent:event andValue:array]];
    }
}

- (void) postData:(id)data withKey:(LynxRenderObjectAttr)key {
    LxUIBody *body = [self getRootUI];
    if (body) {
        [body collectAction:[[LynxUIUpdateDataAction alloc] initWithTarget:_renderObjectImpl andKey:key andData:data]];
    }
}

#pragma mark - LxCoordinatorObject

@synthesize coordinatorTag = _coordinatorTag;
@synthesize coordinatorAffinity = _coordinatorAffinity;

// Reset tag will trigger responder to init
- (void) setCoordinatorTag:(NSString *) tag {
    if (_coordinatorTag && [_coordinatorTag isEqualToString:tag]) return;
    _coordinatorTag = tag;
    [_coordinatorTreatment reset];
    [[self getRootUI] addCoordinatorResponder:self];
}

- (void) setCoodinatorAffinity:(NSString *) affinity {
    if (_coordinatorAffinity && [_coordinatorAffinity isEqualToString:affinity]) return;
    // First remove coordinator in TransferStation
    [[self getRootUI] removeCoordinatorResponder:self];
    if (_coordinatorTypes) {
        [[self getRootUI] removeCoordinatorSponsor:self];
    }
    // Update affinity
    _coordinatorAffinity = affinity;
    // Re-add into TransferStation
    if (affinity && affinity.length != 0) {
        [[self getRootUI] addCoordinatorResponder:self];
        if (_coordinatorTypes) {
            [[self getRootUI] addCoordinatorSponsor:self];
        }
    }
}

#pragma mark - LxCoordinatorSponsor

@synthesize coordinatorTypes = _coordinatorTypes;

- (BOOL)dispatchCoordinatorScrollTop:(NSInteger)scrollTop
                             andLeft:(NSInteger)scrollLeft {
    if (_coordinatorTypes && [_coordinatorTypes hasType:kCoordinatorType_Scroll]) {
        return [[self getRootUI] dispatchNestedActionType:kCoordinatorType_Scroll
                                                  sponsor:self
                                                   params: [NSArray arrayWithObjects: [NSNumber numberWithInteger:scrollTop], [NSNumber numberWithInteger:scrollLeft], nil]];
    }
    return NO;
}

- (BOOL)dispatchCoordinatorTouchEvent:(UIEvent *)event type:(NSString *)type {
    if (_coordinatorTypes && [_coordinatorTypes hasType:kCoordinatorType_Touch]) {
        return [[self getRootUI] dispatchNestedActionType:kCoordinatorType_Touch
                                                  sponsor:self
                                                   params: [NSArray arrayWithObjects: event, type, nil]];
    }
    return NO;
}

- (void) setCoordinatorType:(NSString *) type {
    if (_coordinatorTypes && type && _coordinatorTypes.rawContent && [_coordinatorTypes.rawContent isEqualToString:type]) return;
    if (type && type.length != 0) {
        _coordinatorTypes = [[LxCrdTypes alloc] initWithContent:type];
    } else {
        _coordinatorTypes = nil;
    }
    if (!_coordinatorAffinity) return;
    if (_coordinatorTypes) {
        [[self getRootUI] addCoordinatorSponsor:self];
    } else {
        [[self getRootUI] removeCoordinatorSponsor:self];
    }
}

#pragma mark - LxCrdResponder
@synthesize coordinatorTreatment = _coordinatorTreatment;

- (void) setCoordinatorCommands:(NSString *) commands {
    [_coordinatorTreatment addCoordinatorCommand:commands];
}


@end
