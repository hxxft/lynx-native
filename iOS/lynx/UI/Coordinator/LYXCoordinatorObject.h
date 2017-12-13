// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>

@protocol LYXCoordinatorObject <NSObject>
@required
@property(nonatomic, readonly, copy) NSString *coordinatorAffinity;
@property(nonatomic, readonly, copy) NSString *coordinatorTag;
@end
