// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import "LYXCrdCommandExecutor.h"

@interface LYXCrdPreTreatment : NSObject

- (BOOL) dispatchAction: (NSString *) type
            andExecutor: (LYXCrdCommandExecutor *) executor
                 andTag: (NSString *) tag
              andParams: (NSArray *) params;
@end
