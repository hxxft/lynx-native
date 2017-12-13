// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import "LYXCommandExecutor.h"

@interface LYXPreTreatment : NSObject

- (BOOL) dispatchAction: (NSString *) type
            andExecutor: (LYXCommandExecutor *) executor
                 andTag: (NSString *) tag
              andParams: (NSArray *) params;
@end
