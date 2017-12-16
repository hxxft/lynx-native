// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import "LYXCrdTreatment.h"
#import "LYXCrdObject.h"

@protocol LYXCrdResponder <LYXCrdObject>
@required
@property(nonatomic, readonly) LYXCrdTreatment *coordinatorTreatment;

@end
