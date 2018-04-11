// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef WIDGET_BODY_LYNX_UI_BODY_H_
#define WIDGET_BODY_LYNX_UI_BODY_H_

#include "widget/view/lynx_ui_view.h"
#include "widget/view/view_wrapper.h"

#include "base/ui_action.h"
#include "widget/coordinator/transfer_station.h"

@interface LxUIBody : LxUIView<LYXTransferStation>

- (void) resetView:(ViewWrapper*)view;
- (void) collectAction:(LynxUIAction *)action;
@end

#endif  // WIDGET_BODY_LYNX_UI_BODY_H_
