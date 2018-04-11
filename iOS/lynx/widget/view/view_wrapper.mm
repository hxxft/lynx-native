// Copyright 2017 The Lynx Authors. All rights reserved.

#include "widget/view/view_wrapper.h"

#import <UIKit/UIGestureRecognizerSubclass.h>

@interface GestureHandler () <UIGestureRecognizerDelegate>
@property(nonatomic) BOOL needToBeHandled;
@end

@implementation GestureHandler

- (id)initWithUI:(LynxUI *)ui {
    self = [super init];
    if(self != nil) {
        _ui = ui;
        self.delegate = self;
        _needToBeHandled = NO;
    }
    return self;
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    [super touchesBegan:touches withEvent:event];
    _needToBeHandled = [_ui dispatchCoordinatorTouchEvent:event type:@"touchbegan"];
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    [super touchesMoved:touches withEvent:event];
    _needToBeHandled = [_ui dispatchCoordinatorTouchEvent:event type:@"touchmoved"];
    if (_needToBeHandled) {
        self.state = UIGestureRecognizerStateBegan;
    }
}

- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    [super touchesCancelled:touches withEvent:event];
    [_ui dispatchCoordinatorTouchEvent:event type:@"touchcancelled"];
    self.state = UIGestureRecognizerStateCancelled;
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    [super touchesEnded:touches withEvent:event];
    [_ui dispatchCoordinatorTouchEvent:event type:@"touchended"];
    self.state = UIGestureRecognizerStateEnded;
    self.state = UIGestureRecognizerStateRecognized;
}

- (BOOL)canBePreventedByGestureRecognizer:(UIGestureRecognizer *)preventingGestureRecognizer {
    return _needToBeHandled;
}

@end

@implementation ViewWrapper {
    GestureHandler *swipe;
}

- (void)layoutSubviews {
    [super layoutSubviews];
}

- (id)initWithUI:(LynxUI *)ui {
    self = [super init];
    if(self != nil) {
        self.clipsToBounds = YES;
        _ui = ui;
        _singleTap = [[UITapGestureRecognizer alloc]
                       initWithTarget:self action:@selector(handleSingleTap:)];
    }
    return self;
}

- (void)addEvent:(NSString *)event {
    [self addGestureRecognizer:self.singleTap];
}

- (void)removeEvent:(NSString *)event {
    [self removeGestureRecognizer:self.singleTap];
}

-(void) handleSingleTap:(id)sender {
    NSMutableArray *array = [[NSMutableArray alloc] init];
    NSMutableDictionary *event = [[NSMutableDictionary alloc] init];
    [event setValue:@"click" forKey:@"type"];
    array[0] = event;
    [_ui postEvent:@"click" withValue:array];
}

- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event {
    //首先判断是否可以接收事件
    if (self.userInteractionEnabled == NO || self.hidden == YES || self.alpha <= 0.01) return nil;
    //然后判断点是否在当前视图上
    if ([self pointInside:point withEvent:event] == NO) return nil;
    //循环遍历所有子视图，查找是否有最合适的视图
    for (NSInteger i = self.subviews.count - 1; i >= 0; i--) {
        UIView *childView = self.subviews[i];
        //转换点到子视图坐标系上
        CGPoint childPoint = [self convertPoint:point toView:childView];
        //递归查找是否存在最合适的view
        UIView *fitView = [childView hitTest:childPoint withEvent:event];
        //如果返回非空，说明子视图中找到了最合适的view，那么返回它
        if (fitView) {
            return fitView;
        }
    }

    if ([_ui dispatchCoordinatorTouchEvent:event type:@"touchbegan"]) {
        if (!swipe) {
            swipe = [[GestureHandler alloc] initWithUI:_ui];
            [self addGestureRecognizer:swipe];
        }
        return self;
    }

    //循环结束，仍旧没有合适的子视图可以处理事件，那么就认为自己是最合适的view
    return nil;
}

@end
