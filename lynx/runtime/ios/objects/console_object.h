// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef CONSOLE_OBJECT_H_
#define CONSOLE_OBJECT_H_

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>

@protocol ConsoleObjectProtocol <JSExport>
-(id) time;
-(id) log:(id) value;
-(id) warn:(id) value;
-(id) alert:(id) value;
-(id) error:(id) value;
@end

@interface ConsoleObject : NSObject <ConsoleObjectProtocol>
@end

#endif /* CONSOLE_OBJECT_H_ */
