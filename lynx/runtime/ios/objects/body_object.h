// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef BODY_OBJECT_H_
#define BODY_OBJECT_H_

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>
#import <UIKit/UIKit.h>

#include "scoped_ptr.h"
#include "element_object.h"


@protocol BodyObjectProtocol <ElementObjectProtocol, JSExport>
@property(nonatomic, assign, readonly) CGFloat clientWidth;
@property(nonatomic, assign, readonly) CGFloat clientHeight;
@end

@interface BodyObject : ElementObject <BodyObjectProtocol>
-(id) initWithHost:(lynx::RenderTreeHost*)host;
@end


#endif /* BODY_OBJECT_H_ */
