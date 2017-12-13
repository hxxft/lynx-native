// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import "LYXTreatment.h"
#import "LYXCoordinatorObject.h"

@protocol LYXCoordinatorResponder <LYXCoordinatorObject>
@required
@property(nonatomic, readonly) LYXTreatment *coordinatorTreatment;

@end
