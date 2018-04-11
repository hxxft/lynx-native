// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_OC_PROPERTY_H_
#define UI_BASE_OC_PROPERTY_H_

#import <Foundation/Foundation.h>

enum LYXOcPropertyType {
    LYXOcNoType = 0,
    LYXOcVoidType = 'v',
    LYXOcIntType = 'i',
    LYXOcCharType = 'c',
    LYXOcShortType = 's',
    LYXOcLongType = 'l',
    LYXOcLonglongType = 'q',
    LYXOcFloatType = 'f',
    LYXOcDoubleType = 'd',
    LYXOcBoolType = 'b',
    LYXOcStringType = '*',
    LYXOcNSStringType = 'S',
    LYXOcNSNumberType = 'N',
    LYXOcNSArrayType = 'A',
    LYXOcNSDictionaryType = 'D'
};

@interface LYXOcPropertyInfo : NSObject
@property(nonatomic, readwrite) NSString* typeStr;
@property(nonatomic, readwrite) const char* objCType;
@end

@interface LYXOcProperty : NSObject

- (instancetype) initWithInfos:(NSArray<LYXOcPropertyInfo*>*)infos NS_DESIGNATED_INITIALIZER;
- (instancetype) initWithInfo:(LYXOcPropertyInfo*)info NS_DESIGNATED_INITIALIZER;
- (NSInteger) length;
- (NSString*) transformedTypes;
- (BOOL) isMatchedWith:(NSArray *)args;
- (int) convertProperty:(NSArray*) array toSuitable:(void*[]) args;

@end

#endif  // UI_BASE_OC_PROPERTY_H_

