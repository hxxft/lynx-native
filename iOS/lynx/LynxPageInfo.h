//
//  LynxPageInfo.h
//  lynx
//
//  Created by dli on 2017/9/29.
//  Copyright © 2017年 lynx. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface LynxPageInfo : NSObject

@property (readonly, nonatomic) NSString* name;

- (id) initWithDict:(NSDictionary*)dict;

@end
