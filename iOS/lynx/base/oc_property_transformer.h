// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_OC_PROPERTY_TRANSFORMER_H_
#define UI_BASE_OC_PROPERTY_TRANSFORMER_H_

#import <Foundation/Foundation.h>
#include "base/oc_property.h"

@interface LYXOcPropertyTransformer : NSObject

+ (NSString *) transformType:(LYXOcPropertyInfo*) info;
+ (NSString *) transformTypes:(NSArray<LYXOcPropertyInfo*>*) infos;
+ (NSString *) transformTypeForObject:(id) object;
+ (NSString *) transformTypeForObjects:(NSArray*) object;

@end

#endif  // UI_BASE_OC_PROPERTY_TRANSFORMER_H_
