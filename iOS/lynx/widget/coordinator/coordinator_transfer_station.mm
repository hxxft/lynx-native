// Copyright 2017 The Lynx Authors. All rights reserved.

#include "widget/coordinator/coordinator_transfer_station.h"
#include "widget/coordinator/coordinator_pretreatment.h"

@interface LxCrdTransferStation()
// TransferStation
// affinity <-> responders
@property(nonatomic) NSMutableDictionary<NSString*, NSMutableSet<id<LxCrdResponder>>*> *coordinatorResponders;
// affinity <-> sponsor
@property(nonatomic) NSMutableDictionary<NSString*, id<LxCrdSponsor>> *coordinatorSponsors;
// affinity <-> executor
@property(nonatomic) NSMutableDictionary<NSString*, LxCrdCommandExecutor*> *executorPool;
// sponsor affinity <-> responder affinity
@property(nonatomic) NSMutableDictionary<NSString*, NSMutableSet<NSString*>*> *affinityRelationShip;
@property(nonatomic) LxCrdPreTreatment *preTreatment;

@end;

@implementation LxCrdTransferStation

- (void)updateProperties:(NSDictionary *)object
         sponsorAffinity:(NSString *)sponsorAffinity
       responderAffinity:(NSString *)responderAffinity
                  notify:(BOOL)notify {
    if (_coordinatorResponders && _executorPool && _affinityRelationShip) {
        NSSet<NSString *> *responderAffinityList = _affinityRelationShip[sponsorAffinity];
        if (!responderAffinityList) return;
        LxCrdCommandExecutor *executor = _executorPool[responderAffinity];
        NSSet<id<LxCrdResponder>> *set = _coordinatorResponders[responderAffinity];
        if (!executor || !set) return;
        for (NSString * name in object.allKeys) {
            id value = object[name];
            if ([value isKindOfClass:[NSString class]]) {
                [executor updateProperty:name withStr:value];
            } else if ([value isKindOfClass:[NSNumber class]]) {
                NSNumber *number = value;
                if (number.objCType[0] == 'c') {
                    [executor updateProperty:name withBool:number.boolValue];
                } else {
                    [executor updateProperty:name withDouble:number.doubleValue];
                }
            }
        }
        if (notify) {
            // Notify responder
            for (id<LxCrdResponder> responder : set) {
                [responder.coordinatorTreatment onPropertiesUpdated:executor];
            }
            // Notify sponsor which is a responder
            id<LxCrdSponsor> sponsor = _coordinatorSponsors[sponsorAffinity];
            if (sponsor && [sponsor conformsToProtocol:@protocol(LxCrdResponder)]) {
                LxCrdTreatment *sponsorTreatment = ((id<LxCrdResponder>) sponsor).coordinatorTreatment;
                if (sponsorTreatment) {
                    [sponsorTreatment onPropertiesUpdated:executor];
                }
            }
        }
    }
}

- (void) addExecutableAction:(NSString *) executable
             sponsorAffinity:(NSString *) sponsorAffinity
           responderAffinity:(NSString *) responderAffinity {
    if (!_affinityRelationShip) {
        _affinityRelationShip = [[NSMutableDictionary alloc] init];
    }
    if (!_executorPool) {
        _executorPool = [[NSMutableDictionary alloc] init];
    }
    NSMutableSet<NSString*> *responderAffinityList = _affinityRelationShip[sponsorAffinity];
    if (!responderAffinityList) {
        responderAffinityList = [[NSMutableSet alloc] init];
        [_affinityRelationShip setObject:responderAffinityList forKey:sponsorAffinity];
    }
    [responderAffinityList addObject:responderAffinity];
    
    LxCrdCommandExecutor *executor = _executorPool[responderAffinity];
    if (!executor) {
        executor = [[LxCrdCommandExecutor alloc] initWithExecutableContent:executable];
        [_executorPool setObject:executor forKey:responderAffinity];
    } else {
        [executor updateExecutableContent:executable];
    }
    
    if (!_preTreatment) {
        _preTreatment = [[LxCrdPreTreatment alloc] init];
    }
    // Init responder
    if (_coordinatorResponders) {
        NSMutableSet<id<LxCrdResponder>> *set = _coordinatorResponders[responderAffinity];
        if (set) {
            for (id<LxCrdResponder> responder in set) {
                [responder.coordinatorTreatment initialize:executor];
            }
        }
    }
    // Init sponsor which is a responder
    if (_coordinatorSponsors) {
        id<LxCrdSponsor> sponsor = _coordinatorSponsors[sponsorAffinity];
        if (sponsor && [sponsor conformsToProtocol:@protocol(LxCrdResponder)]) {
            LxCrdTreatment *sponsorTreatment = ((id<LxCrdResponder>) sponsor).coordinatorTreatment;
            if (sponsorTreatment) {
                [sponsorTreatment initialize:executor];
            }
        }
    }
}

- (void) removeExecutableActionWithSponsorAffinity:(NSString *) sponsorAffinity
                                 responderAffinity:(NSString *) responderAffinity {
    if (!_affinityRelationShip || !_executorPool) {
        return;
    }
    NSMutableSet<NSString*> *responderAffinityList = _affinityRelationShip[sponsorAffinity];
    if (responderAffinityList) {
        [responderAffinityList removeObject:responderAffinity];
        [_executorPool removeObjectForKey:responderAffinity];
    }
}

- (void) removeAllExecutableAction {
    [_affinityRelationShip removeAllObjects];
    [_executorPool removeAllObjects];
}

- (void) addCoordinatorSponsor:(id<LxCrdSponsor>) sponsor {
    if (!_coordinatorSponsors) {
        _coordinatorSponsors = [[NSMutableDictionary alloc] init];
    }
    if (_coordinatorSponsors[sponsor.coordinatorAffinity]) return;
    [_coordinatorSponsors setObject:sponsor forKey:sponsor.coordinatorAffinity];
    
    // Init sponsor which is a responder
    if (!_affinityRelationShip) return;
    NSMutableSet<NSString*> *responderAffinityList = _affinityRelationShip[sponsor.coordinatorAffinity];
    LxCrdTreatment *sponsorTreatment = nil;
    if ([sponsor conformsToProtocol:@protocol(LxCrdResponder)]) {
        sponsorTreatment = ((id<LxCrdResponder>) sponsor).coordinatorTreatment;
    }
    if (!responderAffinityList || !sponsorTreatment) return;
    for (NSString *responderAffinity : responderAffinityList) {
        LxCrdCommandExecutor *executor = _executorPool[responderAffinity];
        if (executor) [sponsorTreatment initialize:executor];
    }
}

- (void) removeCoordinatorSponsor:(id<LxCrdSponsor>) sponsor {
    if (_coordinatorSponsors) {
        [_coordinatorSponsors removeObjectForKey:sponsor.coordinatorAffinity];
    }
}

- (void) addCoordinatorResponder:(id<LxCrdResponder>) responder {
    if (!responder.coordinatorAffinity) return;
    if (!_coordinatorResponders) {
        _coordinatorResponders = [[NSMutableDictionary alloc] init];
    }
    NSMutableSet<id<LxCrdResponder>> *set = _coordinatorResponders[responder.coordinatorAffinity];
    if (!set) {
        set = [[NSMutableSet alloc] init];
        [_coordinatorResponders setObject:set forKey:responder.coordinatorAffinity];
    }
    [set addObject:responder];
    // Init responder
    if (_executorPool) {
        LxCrdCommandExecutor *executor = _executorPool[responder.coordinatorAffinity];
        if (executor) {
            [responder.coordinatorTreatment initialize:executor];
        }
    }
}

- (void) removeCoordinatorResponder:(id<LxCrdResponder>) responder {
    if (_coordinatorResponders && responder) {
        NSString *affinity = [responder coordinatorAffinity];
        NSMutableSet<id<LxCrdResponder>>* array = _coordinatorResponders[affinity];
        if (array) {
            [array removeObject:responder];
        }
    }
}

- (BOOL) dispatchNestedActionType: (NSString *) type
                          sponsor: (id<LxCrdSponsor>) sponsor
                           params: (NSArray *) params {
    BOOL consumed = NO;
    if (_coordinatorResponders && _executorPool && _affinityRelationShip) {
        NSMutableSet<NSString*> *responderAffinityList = _affinityRelationShip[sponsor.coordinatorAffinity];;
        if (!responderAffinityList) return consumed;
        
        LxCrdTreatment *sponsorTreatment = nil;
        if ([sponsor conformsToProtocol:@protocol(LxCrdResponder)]) {
            sponsorTreatment = ((id<LxCrdResponder>) sponsor).coordinatorTreatment;
        }
        
        for (NSString *responderAffinity in responderAffinityList) {
            LxCrdCommandExecutor *executor = _executorPool[responderAffinity];
            NSMutableSet<id<LxCrdResponder>> *set = _coordinatorResponders[responderAffinity];
            if (!executor || !set) continue;
            // First call onDispatch
            consumed = [_preTreatment dispatchAction:type
                                         andExecutor:executor
                                              andTag:sponsor.coordinatorTag
                                           andParams:params];
            if (sponsorTreatment) {
                [sponsorTreatment onNestedActionWithType:type andExecutor:executor andArgs:params];
            }
            for (id<LxCrdResponder> responder in set) {
                [responder.coordinatorTreatment onNestedActionWithType:type andExecutor:executor andArgs:params];
            }
        }
    }
    return consumed;
}

@end
