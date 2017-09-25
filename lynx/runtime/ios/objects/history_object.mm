// Copyright 2017 The Lynx Authors. All rights reserved.

#include "history_object.h"
#include "jscore_runtime.h"
#include "common.h"

@implementation HistoryObject

@synthesize length;

-(NSInteger) length
{
    return [self->history_list_ count];
}

-(void) back
{
    [self go:[NSNumber numberWithInt:-1]];
}

-(void) forward
{
    [self go:[NSNumber numberWithInt:1]];
}

-(void) go:(id) param
{
    if ([param isKindOfClass:[NSString class]]) {
        NSString *url = SAFE_CONVERT(param, NSString);
        [self goToUrl:url];
    } else {
        NSInteger num = (NSInteger) param;
        [self goToOffset:num];
    }
    if (pos_ == m_blank_pos) {
        if (runtime_.IsValid()) {
            runtime_.Get()->LoadUrl("");
        }
    } else {
        NSString *url = [self->history_list_ objectAtIndex:pos_];
        if (runtime_.Get()) {
            runtime_.Get()->LoadUrl(std::string([url UTF8String]));
        }
    }
}

-(void) goToUrl:(NSString *)url
{
    NSInteger index = [self->history_list_ indexOfObject:url];
    if (index > m_blank_pos) {
        pos_ = index;
    } else {
        [self->history_list_ addObject:url];
        pos_++;
    }
}

-(void) goToOffset:(NSInteger)offset
{
    NSInteger target = pos_ + offset;
    if (target < -1 || target >= self.length) {
        return;
    }
    pos_ = target;
}

-(void) reloadUrl:(NSString *)url
{
    if (pos_ == m_blank_pos) {
        return;
    }
    [history_list_ replaceObjectAtIndex:pos_ withObject:url];
}

-(id) initWithRuntime:(base::WeakPtr<jscore::JSCoreRuntime>)runtime
{
    self = [super init];
    if(self != nil) {
        self->runtime_ = runtime;
        self->history_list_ = [[NSMutableArray alloc] init];
        pos_ = m_blank_pos;
    }
    return self;
}

@end
