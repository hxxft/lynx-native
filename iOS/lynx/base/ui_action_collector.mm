// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/ui_action_collector.h"

#include "base/lynx_render_object_impl.h"
#include "base/ui_event_action.h"
#include "base/ui_update_data_action.h"

#include "base/ios/common.h"

@implementation LynxUIActionCollector

- (instancetype)init {
    self = [super init];
    if (self) {
        _eventActions = [[NSMutableDictionary alloc] init];
        _updateDataActions = [[NSMutableDictionary alloc] init];
    }
    return self;
}

- (void)collectAction:(LynxUIAction *)action {
    switch (action.type) {
        case DO_EVENT_ACTION: {
            NSValue *key = [NSValue valueWithNonretainedObject:action.renderObjectImpl];
            NSMutableDictionary *dic = _eventActions[key];
            if (!dic) {
                dic = [[NSMutableDictionary alloc] init];
                [_eventActions setObject:dic forKey:key];
            }
            [dic setObject:action forKey:[SAFE_CONVERT(action, LynxUIEventAction) event]];
        }
            break;
        case DO_UPDATE_DATA_ACTION: {
            NSValue *key = [NSValue valueWithNonretainedObject:action.renderObjectImpl];
            NSMutableDictionary *dic = _updateDataActions[key];
            if (!dic) {
                dic = [[NSMutableDictionary alloc] init];
                [_updateDataActions setObject:dic forKey:key];
            }
            [dic setObject:action forKey:[NSNumber numberWithInt: [SAFE_CONVERT(action, LynxUIUpdateDataAction) key]]];
        }
            break;
        case DO_EVENT_NONE:
            break;
        default:
            break;
    }
}

- (BOOL)needDoAction {
    return !(_eventActions.count == 0 && _updateDataActions.count == 0);
}

- (void)doActions {
    for (NSValue *key1 in _eventActions) {
        NSMutableDictionary *dic1 = _eventActions[key1];
        for (NSString *key2 in dic1) {
            LynxUIAction *action = SAFE_CONVERT(dic1[key2], LynxUIAction);
            [action doAction];
        }
    }
    for (NSValue *key1 in _updateDataActions) {
        NSMutableDictionary *dic1 = _updateDataActions[key1];
        for (NSNumber *key2 in dic1) {
            LynxUIAction *action = SAFE_CONVERT(dic1[key2], LynxUIAction);
            [action doAction];
        }
    }
}

@end
