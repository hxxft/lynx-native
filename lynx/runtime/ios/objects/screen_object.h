// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef screen_object_h
#define screen_object_h

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>
#import <UIKit/UIKit.h>

@protocol ScreenObjectProtocol <JSExport>

@property(nonatomic, assign, readonly) CGFloat width;
@property(nonatomic, assign, readonly) CGFloat height;

@end

@interface ScreenObject : NSObject <ScreenObjectProtocol>

@end

#endif /* screen_object_h */
