// Copyright 2017 The Lynx Authors. All rights reserved.

#include "widget/body/lynx_ui_body.h"
#include "widget/body/lynx_content_view.h"
#include "widget/coordinator/coordinator_transfer_station.h"

@interface LxUIBody()
@property(nonatomic)  LYXCrdTransferStation *tranferStationImpl;
@end;

@implementation LxUIBody

#pragma mark - LxUIBody

- (id) initWithRenderObjectImpl:(LynxRenderObjectImpl *) impl {
    self = [super initWithRenderObjectImpl:impl];
    if (self) {
        _tranferStationImpl = [[LYXCrdTransferStation alloc] init];
    }
    return self;
}

- (UIView *)createView:(LynxRenderObjectImpl *)impl {
    return nil;
}

- (void)bindRenderObjectImpl:(LynxRenderObjectImpl *)impl {
    self.renderObjectImpl = impl;
}

- (void) resetView:(ViewWrapper*) view {
    self.view = view;
    // Actually bind data
    [super bindRenderObjectImpl: self.renderObjectImpl];
}

- (void) collectAction:(LynxUIAction *)action {
    [((LxContentView *)self.view).renderTreeHostImpl collectAction:action];
}

#pragma mark - LYXTransferStation

- (void) addExecutableAction:(NSString *) executable
             sponsorAffinity:(NSString *) sponsorAffinity
           responderAffinity:(NSString *) responderAffinity {
    [_tranferStationImpl addExecutableAction:executable
                             sponsorAffinity:sponsorAffinity
                           responderAffinity:responderAffinity];
}

- (void) removeExecutableActionWithSponsorAffinity:(NSString *) sponsorAffinity
                                 responderAffinity:(NSString *) responderAffinity {
    [_tranferStationImpl removeExecutableActionWithSponsorAffinity: sponsorAffinity
                                                 responderAffinity: responderAffinity ];
}

- (void) removeAllExecutableAction {
    [_tranferStationImpl removeAllExecutableAction];
}

- (void) addCoordinatorSponsor:(id<LYXCrdSponsor>) sponsor {
    [_tranferStationImpl addCoordinatorSponsor:sponsor];
}

- (void) removeCoordinatorSponsor:(id<LYXCrdSponsor>) sponsor {
    [_tranferStationImpl removeCoordinatorSponsor:sponsor];
}

- (void) addCoordinatorResponder:(id<LYXCrdResponder>) responder {
    [_tranferStationImpl addCoordinatorResponder:responder];
}

- (void) removeCoordinatorResponder:(id<LYXCrdResponder>) responder {
    [_tranferStationImpl removeCoordinatorResponder:responder];
}

- (BOOL) dispatchNestedActionType: (NSString *) type
                          sponsor: (id<LYXCrdSponsor>) child
                           params: (NSArray *) params {
    return [_tranferStationImpl dispatchNestedActionType: type
                                                 sponsor: child
                                                  params: params];
    
}

- (void)updateProperties:(NSDictionary *)object
         sponsorAffinity:(NSString *)sponsorAffinity
       responderAffinity:(NSString *)responderAffinity
                  notify:(BOOL)notify {
    [_tranferStationImpl updateProperties:object
                          sponsorAffinity:sponsorAffinity
                        responderAffinity:responderAffinity
                                   notify:notify];
}


@end
