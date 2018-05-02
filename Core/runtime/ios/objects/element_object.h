// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef ELEMENT_OBJECT_H_
#define ELEMENT_OBJECT_H_

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>

#include "scoped_ptr.h"
#include "render_object.h"

@protocol ElementObjectProtocol <JSExport>
@property(nonatomic, strong, readonly ) NSString* tagName;
@property(nonatomic, strong, readonly ) id        parentNode;
@property(nonatomic, assign, readonly ) NSInteger nodeType;
@property(nonatomic, assign, readonly ) NSInteger offsetTop;
@property(nonatomic, assign, readonly ) NSInteger offsetLeft;
@property(nonatomic, assign, readonly ) NSInteger offsetWidth;
@property(nonatomic, assign, readonly ) NSInteger offsetHeight;
@property(nonatomic, assign, readonly ) NSInteger scrollWidth;
@property(nonatomic, assign, readonly ) NSInteger scrollHeight;
@property(nonatomic, assign, readwrite ) NSInteger scrollTop;
@property(nonatomic, assign, readwrite ) NSInteger scrollLeft;

-(id) setStyle:(id)style;
-(id) setAttribution:(id)attribution;
-(id) appendChild:(id)child;
-(id) appendChildren:(id)children;
-(id) insertChild:(id)child AtIndex:(id)index;
-(id) getChildByIndex:(id)index;
-(id) removeChild:(id)child;
-(id) removeChildByIndex:(id)index;
-(id) setText:(id)text;

JSExportAs(setAttribute, -(id)setAttribute:(NSString*)key withValue:(id)value);
JSExportAs(insertBefore, -(id)insertChild:(id)child Before:(id)reference);
JSExportAs(addEventListener, -(id)addEvent:(NSString*)event Listener:(JSValue*)callback);
JSExportAs(removeEventListener, -(id)removeEvent:(NSString*)event Listener:(JSValue*)callback);
@end

@interface ElementObject : NSObject <ElementObjectProtocol> {
@protected
    base::ScopedPtr<lynx::RenderObject> render_object_;
}
-(id) initWithTagName:(NSString *)tag_name threadProxy:(jscore::ThreadManager*)thread_proxy host:(lynx::RenderTreeHost*)host;
@end

#endif /* ELEMENT_OBJECT_H_ */
