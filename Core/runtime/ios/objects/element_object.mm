// Copyright 2017 The Lynx Authors. All rights reserved.

#include "common.h"
#include "element_object.h"

#include "render_factory.h"
#include "js_function_wrap.h"

using namespace std;

@implementation ElementObject

- (id)initWithTagName:(NSString *)tag_name
          threadProxy:(jscore::ThreadManager*)thread_proxy
                 host:(lynx::RenderTreeHost*)host {
    self = [super init];
    if(self != nil) {
        const char* name = [tag_name UTF8String];
        std::string str_name(name);
        render_object_.Reset(lynx::RenderFactory::CreateRenderObject(thread_proxy, str_name, host));
        render_object_->SetJSRef((__bridge void *)(self));
    }
    return self;
}

-(void) dealloc {
    NSLog(@"dealloc element");
}


-(id) setStyle:(id)style {
    NSDictionary* dict = SAFE_CONVERT(style, NSDictionary);
    if(dict != nil) {
        NSEnumerator *enumerator = [dict keyEnumerator];
        id key;
        while ((key = [enumerator nextObject])) {
            string str_key([key UTF8String]);
            id value = [dict objectForKey:key];
            
            if([value isKindOfClass:[NSString class]]) {
                string str_value([value UTF8String]);
                render_object_->SetStyle(str_key, str_value);
            }else if([value isKindOfClass:[NSNumber class]]){
                NSNumber* number = SAFE_CONVERT(value, NSNumber);
                string str_value([number.description UTF8String]);
                render_object_->SetStyle(str_key, str_value);
            }
        }
        render_object_->SetStyle("", "");
    }
    return nil;
}

-(id) setText:(id)text {
    NSString* label_text = SAFE_CONVERT(text, NSString);
    string str_text([label_text UTF8String]);
    render_object_->SetText(str_text);
    return nil;
}

-(id) appendChild:(id)child {
    ElementObject* child_element = SAFE_CONVERT(child, ElementObject);
    if(child_element != nil) {
        render_object_->AppendChild(child_element->render_object_.Get());
    }
    return nil;
}

-(id) appendChildren:(id)children {
    NSMutableArray* children_element = SAFE_CONVERT(children, NSMutableArray);
    for (id child : children_element) {
        ElementObject* child_element = SAFE_CONVERT(child, ElementObject);
        [self appendChild: child_element];
    }
    return nil;
}

-(id) insertChild:(id)child AtIndex:(id)index {
    if(child == nil || index == nil)
        return nil;
    ElementObject* child_element = SAFE_CONVERT(child, ElementObject);
    NSNumber* child_index = SAFE_CONVERT(index, NSNumber);
    if(child_element != nil && child_index != nil) {
        render_object_->InsertChild(child_element->render_object_.Get(), [child_index intValue]);
    }
    return nil;
}

-(id) insertChild:(id)child Before:(id)reference {
    if (!child) return nil;
    ElementObject* child_element = SAFE_CONVERT(child, ElementObject);
    if (reference) {
        ElementObject* reference_element = SAFE_CONVERT(reference, ElementObject);
        if(child_element != nil && reference_element != nil) {
            render_object_->InsertBefore(child_element->render_object_.Get(), reference_element->render_object_.Get());
        }
    } else {
        [self appendChild:child];
    }
    return nil;
}

-(id) getChildByIndex:(id)index {
    if (render_object_.Get()) {
        NSNumber* child_index = SAFE_CONVERT(index, NSNumber);
        lynx::RenderObject* element_found = const_cast<lynx::RenderObject*>(render_object_->Get([child_index intValue]));
        if (element_found && element_found->GetJSRef()) {
            return (__bridge ElementObject *)(element_found->GetJSRef());
        }
    }
    return nil;
}


-(id) removeChildByIndex:(id)index {
    if (render_object_.Get()) {
        NSNumber* child_index = SAFE_CONVERT(index, NSNumber);
        render_object_->RemoveChildByIndex([child_index intValue]);
    }
    return nil;
}

-(id) removeChild:(id) child {
    ElementObject* child_object = SAFE_CONVERT(child, ElementObject);
    if(render_object_.Get() && child_object) {
        render_object_->RemoveChild(child_object->render_object_.Get());
    }
    return nil;
}

-(id) setAttribution:(id)attribution {
    NSDictionary* dict = SAFE_CONVERT(attribution, NSDictionary);
    if(dict != nil) {
        NSEnumerator *enumerator = [dict keyEnumerator];
        id key;
        while ((key = [enumerator nextObject])) {
            string str_key([key UTF8String]);
            id value = [dict objectForKey:key];
            if([value isKindOfClass:[NSString class]]) {
                string str_value([value UTF8String]);
                render_object_->SetAttribute(str_key, str_value);
            }else {
                NSNumber* number = SAFE_CONVERT(value, NSNumber);
                // 转化成string
            }
        }
    }
    return nil;
}

-(id) setAttribute:(NSString*)key withValue:(id)value {
    string str_key([key UTF8String]);
    if([value isKindOfClass:[NSString class]]) {
        string str_value([value UTF8String]);
        render_object_->SetAttribute(str_key, str_value);
    }
    return nil;
}

-(id) addEvent:(NSString *)event Listener:(JSValue *)callback {
    if(render_object_.Get()) {
        std::string event_str([event UTF8String]);
        render_object_->AddEventListener(event_str, new jscore::JSFunctionWrap(callback));
    }
    return nil;
}

-(id) removeEvent:(NSString*)event Listener:(JSValue*)callback {
    if (render_object_.Get()) {
        std::string event_str([event UTF8String]);
        render_object_->RemoveEventListener(event_str, new jscore::JSFunctionWrap(callback));
    }
    return nil;
}

-(id) parentNode {
    if(render_object_.Get() == NULL || render_object_->Parent() == NULL)
        return nil;
    lynx::RenderObject* parent_element = const_cast<lynx::RenderObject*>(render_object_->Parent());
    void* parent = parent_element->GetJSRef();
    return (__bridge ElementObject *)(parent);
}

-(id) tagName {
    if (render_object_.Get()) {
        return [NSString stringWithUTF8String:render_object_->tag_name().c_str()];
    }
    return @"";
}

-(NSInteger) nodeType
{
    return 1;
}

-(NSInteger) offsetTop
{
    if (render_object_.Get()) {
        return render_object_.Get()->offset_top();
    }
    return 0;
}

-(NSInteger) offsetLeft
{
    if (render_object_.Get()) {
        return render_object_.Get()->offset_left();
    }
    return 0;
}

-(NSInteger) offsetWidth
{
    if (render_object_.Get()) {
        return render_object_.Get()->offset_width();
    }
    return 0;
}

-(NSInteger) offsetHeight
{
    if (render_object_.Get()) {
        return render_object_.Get()->offset_height();
    }
    return 0;
}

-(NSInteger) scrollWidth
{
    if (render_object_.Get()) {
        return render_object_.Get()->scroll_width();
    }
    return 0;
}

-(NSInteger) scrollHeight
{
    if (render_object_.Get()) {
        return render_object_.Get()->scroll_height();
    }
    return 0;
}

-(NSInteger) scrollTop
{
    if (render_object_.Get()) {
        return render_object_.Get()->scroll_top();
    }
    return 0;
}

-(void)setScrollTop:(NSInteger)scrollTop
{
    if (render_object_.Get()) {
        render_object_->SetScrollTop(scrollTop);
    }
}

-(NSInteger) scrollLeft
{
    if (render_object_.Get()) {
        return render_object_.Get()->scroll_left();
    }
    return 0;
}

-(void)setScrollLeft:(NSInteger)scrollLeft
{
    if (render_object_.Get()) {
        render_object_->SetScrollLeft(scrollLeft);
    }
}

@end
