// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_WIDGET_COORDINATOR_COORDINATOR_RESPONSOR_H_
#define UI_WIDGET_COORDINATOR_COORDINATOR_RESPONSOR_H_

#import <Foundation/Foundation.h>

#include "widget/coordinator/coordinator_treatment.h"
#include "widget/coordinator/coordinator_object.h"

@protocol LxCrdResponder <LxCrdObject>
@required
@property(nonatomic, readonly) LxCrdTreatment *coordinatorTreatment;

@end

#endif  // UI_WIDGET_COORDINATOR_COORDINATOR_RESPONSOR_H_
