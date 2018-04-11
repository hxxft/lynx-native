// Copyright 2017 The Lynx Authors. All rights reserved.

#include "modules/coordinator_register.h"
#include "widget/coordinator/coordinator_transfer_station.h"
#include "widget/body/lynx_ui_body.h"

#include "base/ios/common.h"

@implementation LYXCrdRegister {
    LynxRuntime* _runtime;
}

- (instancetype)initWithRuntime:(LynxRuntime *) runtime {
    self = [super init];
    if (self) {
        _runtime = runtime;
    }
    return self;
}

LYX_REGISTER_MODULE(CoordinatorRegister)

LYX_REGISTER_METHOD_WName(registerAction, void,
                          registerSponsorAffinity: (NSString*) sponsorAffinity
                                responderAffinity: (NSString *) responderAffinity
                                           action: (NSString *) action) {
    dispatch_sync(dispatch_get_main_queue(), ^{
        LxUIBody *body = (LxUIBody *) _runtime.host.rootRenderObjectImpl.ui;
        [body addExecutableAction:action
                  sponsorAffinity:sponsorAffinity
                responderAffinity:responderAffinity];
    });
}

LYX_REGISTER_METHOD_WName(removeAction, void,
                          removeActionBySponsorAffinity:(NSString*) sponsorAffinity
                                      responderAffinity: (NSString *) responderAffinity) {
    dispatch_sync(dispatch_get_main_queue(), ^{
        LxUIBody *body = (LxUIBody *) _runtime.host.rootRenderObjectImpl.ui;
        [body removeExecutableActionWithSponsorAffinity:sponsorAffinity
                                      responderAffinity:responderAffinity];
    });
}

LYX_REGISTER_METHOD_WName(updateProperties, void,
                          updateSponsorAffinity: (NSString*) sponsorAffinity
                              responderAffinity: (NSString *) responderAffinity
                                     properties: (NSDictionary *) object
                                         notify: (BOOL) notify) {
    dispatch_sync(dispatch_get_main_queue(), ^{
        LxUIBody *body = (LxUIBody *) _runtime.host.rootRenderObjectImpl.ui;
        [body updateProperties:object
               sponsorAffinity:sponsorAffinity
             responderAffinity:responderAffinity
                        notify:notify];
    });
}

@end
