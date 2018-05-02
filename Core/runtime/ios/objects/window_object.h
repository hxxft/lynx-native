// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef window_object_h
#define window_object_h

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>
#import <UIKit/UIKit.h>

@interface WindowObject : NSObject

@property (nonatomic, assign, readonly) CGFloat devicePixelRatio;


-(id) initWithContext: (JSContext *)context;
@end

#endif /* window_object_h */
