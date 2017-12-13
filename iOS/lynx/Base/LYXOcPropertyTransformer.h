// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import "LYXOcProperty.h"

@interface LYXOcPropertyTransformer : NSObject

+ (NSString *) transformType:(LYXOcPropertyInfo*) info;
+ (NSString *) transformTypes:(NSArray<LYXOcPropertyInfo*>*) infos;
+ (NSString *) transformTypeForObject:(id) object;
+ (NSString *) transformTypeForObjects:(NSArray*) object;

@end
