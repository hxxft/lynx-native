// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_DEFINES_H_
#define UI_BASE_DEFINES_H_

#import <Foundation/Foundation.h>

#ifndef LX_CONCAT
#define LX_CONCAT2(A, B) A ## B
#define LX_CONCAT(A, B) LX_CONCAT2(A, B)
#endif

#ifndef LX_NOT_IMPLEMENTED
#define LX_NOT_IMPLEMENTED(method) \
method NS_UNAVAILABLE { \
    NSString *msg = [NSString stringWithFormat:@"%s is not implemented in class %@", sel_getName(_cmd), self.class]; \
    @throw [NSException exceptionWithName:@"LxNotDesignatedInitializerException" \
                                   reason:msg \
                                 userInfo:nil]; \
}
#endif

#endif  // UI_BASE_DEFINES_H_
