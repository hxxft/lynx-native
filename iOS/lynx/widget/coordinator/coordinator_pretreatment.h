// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_WIDGET_COORDINATOR_COORDINATOR_PRETREATMENT_H_
#define UI_WIDGET_COORDINATOR_COORDINATOR_PRETREATMENT_H_

#import <Foundation/Foundation.h>

#include "widget/coordinator/coordinator_cmd_executor.h"

@interface LxCrdPreTreatment : NSObject

- (BOOL) dispatchAction: (NSString *) type
            andExecutor: (LxCrdCommandExecutor *) executor
                 andTag: (NSString *) tag
              andParams: (NSArray *) params;
@end

#endif  // UI_WIDGET_COORDINATOR_COORDINATOR_PRETREATMENT_H_
