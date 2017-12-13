// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LYXCoordinatorTypes.h"
#import "LYXDefines.h"

@implementation LYXCoordinatorTypes {
    NSSet *_set;
}
NSString * const kCoordinatorType_Scroll = @"scroll";
NSString * const kCoordinatorType_Touch = @"touch";

LYX_NOT_IMPLEMENTED(- (instancetype) init)

- (instancetype)initWithContent:(NSString *)content {
    self = [super init];
    if (self) {
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
