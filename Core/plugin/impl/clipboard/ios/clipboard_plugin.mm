// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "plugin/impl/clipboard/ios/clipboard_plugin.h"

@implementation ClipboardPlugin


-(id) init {
    self = [super init];
    if(self != nil) {
        self.pluginName = @"Clipboard";
    }
    return self;
}


-(void) getString:(NSInteger)clientId method:(NSNumber*)methodId arguments:(NSArray*) args {
 dispatch_sync(dispatch_get_main_queue(), ^{
    UIPasteboard *clipboard = [UIPasteboard generalPasteboard];
    NSMutableArray* result = [[NSMutableArray alloc]init];
    [result addObject:(clipboard.string ? : @"")];
     [self returnBack:clientId method:methodId successed:true argments:result];
 });
}

-(void) setString:(NSInteger)clientId method:(NSNumber*)methodId arguments:(NSArray*) args {
    dispatch_sync(dispatch_get_main_queue(), ^{
        UIPasteboard *clipboard = [UIPasteboard generalPasteboard];
        clipboard.string = ([args objectAtIndex:0] ? : @"");
    });
}

@end
