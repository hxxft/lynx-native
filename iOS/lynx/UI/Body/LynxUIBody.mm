// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LynxUIBody.h"
#import "LynxView.h"
#import "LYXCrdTransferStation.h"

@interface LynxUIBody()
@property(nonatomic)  LYXCrdTransferStation *tranferStationImpl;
@end;

@implementation LynxUIBody

#pragma mark - LynxUIBody

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

- (void) resetView:(IOSView*) view {
    self.view = view;
    // Actually bind data
    [super bindRenderObjectImpl: self.renderObjectImpl];
}

- (void) collectAction:(LynxUIAction *)action {
    [((LynxView *)self.view).renderTreeHostImpl collectAction:action];
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

- (void) addCoordinatorSponsor:(id<LYXCoordinatorSponsor>) sponsor {
    [_tranferStationImpl addCoordinatorSponsor:sponsor];
}

- (void) removeCoordinatorSponsor:(id<LYXCoordinatorSponsor>) sponsor {
    [_tranferStationImpl removeCoordinatorSponsor:sponsor];
}

- (void) addCoordinatorResponder:(id<LYXCoordinatorResponder>) responder {
    [_tranferStationImpl addCoordinatorResponder:responder];
}

- (void) removeCoordinatorResponder:(id<LYXCoordinatorResponder>) responder {
    [_tranferStationImpl removeCoordinatorResponder:responder];
}

- (BOOL) dispatchNestedActionType: (NSString *) type
                          sponsor: (id<LYXCoordinatorSponsor>) child
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
