// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_OC_PROPERTY_H_
#define UI_BASE_OC_PROPERTY_H_

#import <Foundation/Foundation.h>

enum LxOcPropertyType {
    LxOcNoType = 0,
    LxOcVoidType = 'v',
    LxOcIntType = 'i',
    LxOcCharType = 'c',
    LxOcShortType = 's',
    LxOcLongType = 'l',
    LxOcLonglongType = 'q',
    LxOcFloatType = 'f',
    LxOcDoubleType = 'd',
    LxOcBoolType = 'b',
    LxOcStringType = '*',
    LxOcNSStringType = 'S',
    LxOcNSNumberType = 'N',
    LxOcNSArrayType = 'A',
    LxOcNSDictionaryType = 'D'
};

@interface LxOcPropertyInfo : NSObject
@property(nonatomic, readwrite) NSString* typeStr;
@property(nonatomic, readwrite) const char* objCType;
@end

@interface LxOcProperty : NSObject

- (instancetype) initWithInfos:(NSArray<LxOcPropertyInfo*>*)infos NS_DESIGNATED_INITIALIZER;
- (instancetype) initWithInfo:(LxOcPropertyInfo*)info NS_DESIGNATED_INITIALIZER;
- (NSInteger) length;
- (NSString*) transformedTypes;
- (BOOL) isMatchedWith:(NSArray *)args;
- (int) convertProperty:(NSArray*) array toSuitable:(void*[]) args;

@end

#endif  // UI_BASE_OC_PROPERTY_H_

