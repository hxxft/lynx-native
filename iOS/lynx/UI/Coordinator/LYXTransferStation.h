// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import "LYXCrdResponder.h"
#import "LYXCrdSponsor.h"

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

- (void) addCoordinatorSponsor:(id<LYXCrdSponsor>) sponsor;

- (void) removeCoordinatorSponsor:(id<LYXCrdSponsor>) sponsor;

- (void) addCoordinatorResponder:(id<LYXCrdResponder>) responder;

- (void) removeCoordinatorResponder:(id<LYXCrdResponder>) responder;

- (BOOL) dispatchNestedActionType: (NSString *) type
                          sponsor: (id<LYXCrdSponsor>) sponsor
                           params: (NSArray *) params;

@end
