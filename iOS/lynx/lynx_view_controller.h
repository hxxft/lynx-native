// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_LYNX_VIEW_CONTROLLER_H_
#define UI_LYNX_VIEW_CONTROLLER_H_

#import <UIKit/UIKit.h>

@interface LynxViewController : UIViewController 

@property(copy) NSString* page_name_;

- (id) initWithName:(NSString*) name;

@end

#endif
