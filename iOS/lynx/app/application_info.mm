// Copyright 2017 The Lynx Authors. All rights reserved.

#include "app/application_info.h"
#include "app/page_info.h"

@implementation LynxApplicationInfo

//- (NSString*) getPackageName {
//    return package_name_;
//}

- (id) initWithManifest:(NSDictionary*)manifest {
    self = [super init];
    if(self) {
        NSDictionary* appInfo = [manifest objectForKey:@"application"];
        _packageName = [appInfo objectForKey:@"packageName"];
        _mainPage = [[appInfo objectForKey:@"mainPage"] stringByDeletingPathExtension];
        NSArray* pages = [appInfo objectForKey:@"pages"];
        _pages = [[NSMutableArray alloc]initWithCapacity:((NSArray*)pages).count];
        for(NSDictionary* page in pages) {
            [_pages addObject:[[LynxPageInfo alloc]initWithDict:page]];
        }
    }
    return self;
}

@end
