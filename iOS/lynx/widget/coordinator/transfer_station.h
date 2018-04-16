// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_WIDGET_COORDINATOR_TRANSFER_STATION_H_
#define UI_WIDGET_COORDINATOR_TRANSFER_STATION_H_

#import <Foundation/Foundation.h>

#include "widget/coordinator/coordinator_responder.h"
#include "widget/coordinator/coordinator_sponsor.h"

@protocol LxTransferStation <NSObject>

@required

- (void) updateProperties:(NSDictionary *) object
          sponsorAffinity:(NSString *) sponsorAffinity
        responderAffinity:(NSString *) responderAffinity
                   notify:(BOOL) notify;

- (void) addExecutableAction:(NSString *) executable
             sponsorAffinity:(NSString *) sponsorAffinity
           responderAffinity:(NSString *) responderAffinity;
- (void) removeExecutableActionWithSponsorAffinity:(NSString *) sponsorAffinity
                                 responderAffinity:(NSString *) responderAffinity;
- (void) removeAllExecutableAction;

- (void) addCoordinatorSponsor:(id<LxCrdSponsor>) sponsor;

- (void) removeCoordinatorSponsor:(id<LxCrdSponsor>) sponsor;

- (void) addCoordinatorResponder:(id<LxCrdResponder>) responder;

- (void) removeCoordinatorResponder:(id<LxCrdResponder>) responder;

- (BOOL) dispatchNestedActionType: (NSString *) type
                          sponsor: (id<LxCrdSponsor>) sponsor
                           params: (NSArray *) params;

@end

#endif  // UI_WIDGET_COORDINATOR_TRANSFER_STATION_H_
