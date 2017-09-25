// Copyright 2017 The Lynx Authors. All rights reserved.

#import "IOSView.h"

@implementation IOSView

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

@end
