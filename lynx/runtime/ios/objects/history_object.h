// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef history_object_hpp
#define history_object_hpp

#include <stdio.h>
#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>
#include "uri.h"
#include "base/weak_ptr.h"

namespace jscore {
    class JSCoreRuntime;
}

@protocol HistoryObjectProtocol<JSExport>

@property(nonatomic, assign, readonly) NSInteger length;

// This method loads the previous URL in the history list (if exsit) which equals clicking the Back button or calling history.go(-1).
-(void) back;

// This method loads the next URL in to the history which equals clicking the Forward button or calling history.go(1).
-(void) forward;

// This method uses to load a new document.
-(void) go:(id) url;

@end

static const NSInteger m_blank_pos = 1;

@interface HistoryObject : NSObject<HistoryObjectProtocol>
{
    base::WeakPtr<jscore::JSCoreRuntime> runtime_;

    // Keep the url history
    NSMutableArray *history_list_;

    // The position in history of the current url
    NSInteger pos_;
}
-(void) goToUrl:(NSString *) url;
-(void) goToOffset:(NSInteger) offset;
-(void) reloadUrl:(NSString *) url;
-(id) initWithRuntime:(base::WeakPtr<jscore::JSCoreRuntime>) runtime;
@end

#endif /* history_object_hpp */
