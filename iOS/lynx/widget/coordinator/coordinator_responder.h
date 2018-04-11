// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_WIDGET_COORDINATOR_COORDINATOR_RESPONSOR_H_
#define UI_WIDGET_COORDINATOR_COORDINATOR_RESPONSOR_H_

#import <Foundation/Foundation.h>

#include "widget/coordinator/coordinator_treatment.h"
#include "widget/coordinator/coordinator_object.h"

@protocol LYXCrdResponder <LYXCrdObject>
@required
@property(nonatomic, readonly) LYXCrdTreatment *coordinatorTreatment;

@end

#endif  // UI_WIDGET_COORDINATOR_COORDINATOR_RESPONSOR_H_
