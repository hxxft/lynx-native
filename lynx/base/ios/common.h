// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef COMMON_H_
#define COMMON_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#define iOS9Later ([UIDevice currentDevice].systemVersion.floatValue >= 9.0f)

#define SAFE_CONVERT(_object, _t)                                          \
_object? ([_object isKindOfClass:[_t class]]? (_t*)(_object): nil): nil

#define COLOR_CONVERT(color)    \
([[UIColor alloc]initWithRed:color.r_/255.0 green:color.g_/255.0 blue:color.b_/255.0 alpha:color.a_])

#endif /* COMMON_H_ */
