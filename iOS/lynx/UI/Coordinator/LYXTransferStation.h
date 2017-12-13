// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import "LYXCoordinatorResponder.h"
#import "LYXCoordinatorSponsor.h"

@protocol LYXTransferStation <NSObject>

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

- (void) addCoordinatorSponsor:(id<LYXCoordinatorSponsor>) sponsor;

- (void) removeCoordinatorSponsor:(id<LYXCoordinatorSponsor>) sponsor;

- (void) addCoordinatorResponder:(id<LYXCoordinatorResponder>) responder;

- (void) removeCoordinatorResponder:(id<LYXCoordinatorResponder>) responder;

- (BOOL) dispatchNestedActionType: (NSString *) type
                          sponsor: (id<LYXCoordinatorSponsor>) sponsor
                           params: (NSArray *) params;

@end
