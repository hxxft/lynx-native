// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LYXPreTreatment.h"
#import "LYXCoordinatorTypes.h"
#import "LYXPixelUtil.h"

@implementation LYXPreTreatment
static NSString * const kDispatchScrollCommand = @"onDispatchScrollEvent";
static NSString * const kDispatchTouchCommand = @"onDispatchTouchEvent";
static NSDictionary * const kTouchType = [[NSDictionary alloc] initWithObjects:@[@"0", @"1", @"2", @"3"]
                                                                       forKeys:@[@"touchbegan", @"touchended", @"touchmoved", @"touchcancelled"]];

- (BOOL) dispatchAction: (NSString *) type
            andExecutor: (LYXCommandExecutor *) executor
                 andTag: (NSString *) tag
              andParams: (NSArray *) params {
    if ([type isEqualToString:kCoordinatorType_Scroll]) {
        return [self dispatchScrollTop:params[0]
                               andLeft:params[1]
                                andTag:tag
                           andExecutor:executor];
    } else {
        return [self dispatchTouchEvent:params[0]
                                andType:params[1]
                                 andTag:tag
                            andExecutor:executor];
    }
    return NO;
}

- (BOOL) dispatchScrollTop:(NSNumber *)scrollTop
                   andLeft:(NSNumber *)scrollLeft
                    andTag:(NSString *)tag
               andExecutor:(LYXCommandExecutor *)executor {
    double args[2];
    args[0] = [LYXPixelUtil pxToLynxNumber:scrollTop.intValue];
    args[1] = [LYXPixelUtil pxToLynxNumber:scrollLeft.intValue];
    [executor executeCommandWithMethod:kDispatchScrollCommand
                                andTag:tag
                               andArgs:args
                             andLength:2];
    return NO;
}

- (BOOL) dispatchTouchEvent:(UIEvent *) event
                    andType:(NSString *) type
                     andTag:(NSString *) tag
                andExecutor:(LYXCommandExecutor *)executor {
//        touchbegan            = 0;
//        touchended            = 1;
//        touchmoved            = 2;
//        touchcancelled        = 3;
    double args[4];
    args[0] = [kTouchType[type] intValue];
    args[1] = 0;
    args[2] = 0;
    args[3] = event.timestamp;
    if (event.allTouches && event.allTouches.count != 0) {
        UITouch *touch = [event.allTouches allObjects][0];
        CGPoint location = [touch locationInView:touch.window];
        args[1] = [LYXPixelUtil pxToLynxNumber:location.x];
        args[2] = [LYXPixelUtil pxToLynxNumber:location.y];
    }
    
    return [executor executeCommandWithMethod:kDispatchTouchCommand
                                       andTag:tag
                                      andArgs:args
                                    andLength:2].consumed_;
}

@end
