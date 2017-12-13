// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LynxUI.h"
#import "LynxUIBody.h"
#import "LynxUIEventAction.h"
#import "LynxUIUpdateDataAction.h"
#import "LYXTreatment.h"
#import "LYXDefines.h"
#import "LYXCoordinatorActionExecutor.h"
#import "LYXCoordinatorTypes.h"

#include "base/ios/common.h"
#include "render/render_object_type.h"
#include "render/ios/render_object_impl_ios.h"

static NSString * const kAttrCoordinatorAffinity = @"coordinator-affinity";
static NSString * const kAttrCoordinatorTag = @"coordinator-tag";
static NSString * const kAttrCoordinatorType = @"coordinator-type";


@interface LynxUI()
@property(nonatomic) LYXCoordinatorTypes *coordinatorTypes;
@end

@implementation LynxUI

#pragma mark - LynxUI

LYX_NOT_IMPLEMENTED(- (instancetype)init)

- (id)initWithRenderObjectImpl:(LynxRenderObjectImpl *) impl {
    self = [super init];
    if (self) {
        _view = [self createView:impl];
        _coordinatorTreatment = [[LYXTreatment alloc] initWithUI:self];
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

- (UIView *) createView:(LynxRenderObjectImpl *) impl {
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

- (void) insertChild:(LynxRenderObjectImpl *) child atIndex:(int) index {
    
}

- (void) removeChild:(LynxRenderObjectImpl *) child {
    
}

- (void) setText:(NSString *) text {
    
}

- (void) setAttribute:(NSString *) value forKey:(NSString *) key {
    if ([kAttrCoordinatorTag isEqualToString:key]) {
        [self setCoordinatorTag:value];
    } else if ([kAttrCoordinatorAffinity isEqualToString:key]) {
        [self setCoodinatorAffinity:value];
    } else if ([kAttrCoodinatorCommand isEqualToString:key]) {
        [_coordinatorTreatment addCoordinatorCommand:value];
    } else if ([kAttrCoordinatorType isEqualToString:key]) {
        [self setCoodinatorTypes:value];
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

- (void) bindRenderObjectImpl:(LynxRenderObjectImpl *) impl {
    _renderObjectImpl = impl;
    [self updateStyle:impl->style_];
    [self setPosition:impl->position_];
    [self setAttribution:impl];
    [self addEvents:impl];
}

- (void) setAttribution:(LynxRenderObjectImpl *) impl {
    if (impl.attributes.count != 0) {
        for (id key in impl.attributes) {
            id value = impl.attributes[key];
            [self setAttribute:SAFE_CONVERT(value, NSString)
                        forKey:SAFE_CONVERT(key, NSString)];
        }
    }
}

- (void) addEvents:(LynxRenderObjectImpl *) impl {
    if (impl.events.count != 0) {
        for (id key in impl.events) {
            [self addEventListener:SAFE_CONVERT(key, NSString)];
        }
    }
}

- (void) unbindRenderObjectImpl {
    _renderObjectImpl = nil;
}

- (LynxUIBody *) getRootUI {
    if (_renderObjectImpl && _renderObjectImpl.root) {
        return (LynxUIBody *)_renderObjectImpl.root.ui;
    }
    return nil;
}

- (void) postEvent:(NSString *)event withValue:(NSArray *)array {
    LynxUIBody *body = [self getRootUI];
    if (body) {
        [body collectAction:[[LynxUIEventAction alloc] initWithTarget:_renderObjectImpl andEvent:event andValue:array]];
    }
}

- (void) postData:(id)data withKey:(LynxRenderObjectAttr)key {
    LynxUIBody *body = [self getRootUI];
    if (body) {
        [body collectAction:[[LynxUIUpdateDataAction alloc] initWithTarget:_renderObjectImpl andKey:key andData:data]];
    }
}

#pragma mark - LYXCoordinatorObject

@synthesize coordinatorTag = _coordinatorTag;
@synthesize coordinatorAffinity = _coordinatorAffinity;

- (void) setCoordinatorTag:(NSString *) tag {
    _coordinatorTag = tag;
}

- (void) setCoodinatorAffinity:(NSString *) affinity {
    if (_coordinatorAffinity && ![_coordinatorAffinity isEqualToString:affinity]) {
        [[self getRootUI] removeCoordinatorResponder:self];
        if (_coordinatorTypes) {
            [[self getRootUI] removeCoordinatorSponsor:self];
        }
    }
    _coordinatorAffinity = affinity;
    if (affinity && affinity.length != 0) {
        [[self getRootUI] addCoordinatorResponder:self];
        if (_coordinatorTypes) {
            [[self getRootUI] addCoordinatorSponsor:self];
        }
    }
}

#pragma mark - LYXCoordinatorSponsor

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

- (void) setCoodinatorTypes:(NSString *) type {
    if (type && type.length != 0) {
        _coordinatorTypes = [[LYXCoordinatorTypes alloc] initWithContent:type];
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

#pragma mark - LYXCoordinatorResponder
@synthesize coordinatorTreatment = _coordinatorTreatment;


@end
