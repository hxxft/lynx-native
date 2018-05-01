// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_LYNX_VIEW_CONTROLLER_H_
#define UI_LYNX_VIEW_CONTROLLER_H_

#import <UIKit/UIKit.h>

@interface LynxViewController : UIViewController 

@property(copy) NSString* page_name_;
@property(copy) NSString* protocal_;

- (id) initWithName:(NSString*) name;
- (id) initWithProtocal:(NSString*) protocal forName:(NSString*) name;

@end

#endif
