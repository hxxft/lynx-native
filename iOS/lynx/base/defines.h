// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_DEFINES_H_
#define UI_BASE_DEFINES_H_

#import <Foundation/Foundation.h>

#ifndef LYX_CONCAT
#define LYX_CONCAT2(A, B) A ## B
#define LYX_CONCAT(A, B) LYX_CONCAT2(A, B)
#endif

#ifndef LYX_NOT_IMPLEMENTED
#define LYX_NOT_IMPLEMENTED(method) \
method NS_UNAVAILABLE { \
    NSString *msg = [NSString stringWithFormat:@"%s is not implemented in class %@", sel_getName(_cmd), self.class]; \
    @throw [NSException exceptionWithName:@"LYXNotDesignatedInitializerException" \
                                   reason:msg \
                                 userInfo:nil]; \
}
#endif

#endif  // UI_BASE_DEFINES_H_
