// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_WIDGET_COORDINATOR_COORDINATOR_OBJECT_H_
#define UI_WIDGET_COORDINATOR_COORDINATOR_OBJECT_H_

#import <Foundation/Foundation.h>

@protocol LxCrdObject <NSObject>
@required
@property(nonatomic, readonly, copy) NSString *coordinatorAffinity;
@property(nonatomic, readonly, copy) NSString *coordinatorTag;
@end

#endif  // UI_WIDGET_COORDINATOR_COORDINATOR_OBJECT_H_
