// Copyright 2017 The Lynx Authors. All rights reserved.

#include "widget/coordinator/coordinator_treatment.h"

#include "widget/lynx_ui.h"
#include "utils/pixel_util.h"
#include "base/defines.h"
#include "widget/coordinator/coordinator_types.h"
#include "widget/coordinator/coordinator_commands.h"

#include "widget/coordinator/coordinator_responder.h"
#include "widget/coordinator/coordinator_commands.h"
#include "widget/coordinator/coordinator_action_executor.h"


#include "render/coordinator/coordinator_action.h"

@implementation LYXCrdTreatment {
    BOOL _inited;
    LYXCrdCommands *_commands;
    LYXCrdActionExecutor *_actionExecutor;
}
NSString * const kAttrCoodinatorCommand = @"coordinator-command";
static NSString * const kCommandInit = @"init";
static NSString * const kCommandUpdateProperties = @"onPropertiesUpdated";

LYX_NOT_IMPLEMENTED(-(instancetype) init)

- (instancetype)initWithResponder:(id<LYXCrdResponder>) responder actionExecutor:(LYXCrdActionExecutor *) executor {
    self = [super init];
    if (self) {
        _responder = responder;
        _inited = NO;
        _actionExecutor = executor;
    }
    return self;
}

- (void) addCoordinatorCommand:(NSString *) content {
    _commands = [[LYXCrdCommands alloc] initWithContent:content];
}

- (void) initialize:(LYXCrdCommandExecutor *) executor {
    if (!_inited) {
        _inited = YES;
        lynx::CoordinatorAction action = [executor executeCommandWithMethod:kCommandInit
                                                                     andTag:_responder.coordinatorTag
                                                                    andArgs:NULL
                                                                  andLength:0];
        [_actionExecutor executeAction:action];
    }
}

- (void) reset {
    _inited = false;
}

- (void) onPropertiesUpdated:(LYXCrdCommandExecutor *) executor {
    lynx::CoordinatorAction action = [executor executeCommandWithMethod:kCommandUpdateProperties
                                                                 andTag:_responder.coordinatorTag
                                                                andArgs:NULL
                                                              andLength:0];
    [_actionExecutor executeAction:action];
}

- (BOOL) onNestedScrollWithTop:(NSNumber *) scrollTop
                       andLeft:(NSNumber *) scrollLeft
                   andExecutor:(LYXCrdCommandExecutor *) executor {
    double args[2];
    args[0] = [LYXPixelUtil pxToLynxNumber:scrollTop.intValue];
    args[1] = [LYXPixelUtil pxToLynxNumber:scrollLeft.intValue];
    NSString* command = [_commands getCommand:kCoordinatorType_Scroll];
    if (command) {
        lynx::CoordinatorAction action = [executor executeCommandWithMethod:command
                                                                     andTag:_responder.coordinatorTag
                                                                    andArgs:args
                                                                  andLength:2];
        [_actionExecutor executeAction:action];
    }
    return NO;
}

- (BOOL) onNestedTouchEvenWithExecutor:(LYXCrdCommandExecutor *)executor {
    NSString* command = [_commands getCommand:kCoordinatorType_Touch];
    if (command) {
        double args[2] = {0, 0};
        
        lynx::CoordinatorAction action = [executor executeCommandWithMethod:command
                                                                     andTag:_responder.coordinatorTag
                                                                    andArgs:args
                                                                  andLength:2];
        [_actionExecutor executeAction:action];
        return action.consumed_;
    }
    return NO;
}

- (BOOL) onNestedActionWithType:(NSString *) type
                    andExecutor:(LYXCrdCommandExecutor *) executor
                        andArgs:(NSArray *) args {
    if (_commands) {
        if ([type isEqualToString:kCoordinatorType_Scroll]) {
            return [self onNestedScrollWithTop:args[0]  andLeft:args[1] andExecutor:executor];
        } else if ([type isEqualToString:kCoordinatorType_Touch]) {
            return [self onNestedTouchEvenWithExecutor:executor];
        }
    }
    return NO;
}

@end
