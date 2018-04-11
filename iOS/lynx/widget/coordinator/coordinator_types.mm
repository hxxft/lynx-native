// Copyright 2017 The Lynx Authors. All rights reserved.

#include "widget/coordinator/coordinator_types.h"
#include "base/defines.h"

@implementation LYXCrdTypes {
    NSSet *_set;
}
NSString * const kCoordinatorType_Scroll = @"scroll";
NSString * const kCoordinatorType_Touch = @"touch";

LYX_NOT_IMPLEMENTED(- (instancetype) init)

- (instancetype)initWithContent:(NSString *)content {
    self = [super init];
    if (self) {
        _rawContent = content;
        NSMutableSet *set = [[NSMutableSet alloc] init];
        NSArray *types = [content componentsSeparatedByString:@"\\|"];
        for (NSString * type : types) {
            [set addObject:type];
        }
        _set = set;
    }
    return self;
}

- (BOOL)hasType:(NSString *)type {
    return [_set containsObject:type];
}

@end
