//
//  LynxPageInfo.m
//  lynx
//
//  Created by dli on 2017/9/29.
//  Copyright © 2017年 lynx. All rights reserved.
//

#import "LynxPageInfo.h"

@implementation LynxPageInfo

- (id) initWithDict:(NSDictionary*)dict {
    self = [super init];
    if(self) {
        _name = [[dict objectForKey:@"name"]stringByDeletingPathExtension];
    }
    return self;
}

@end
