// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.coordinator;

import com.lynx.core.base.LynxObject;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

public class CrdTransferStation implements TransferStation {
    // TransferStation
    // affinity <-> responders
    private Map<String, Set<CoordinatorResponder>> mCoordinatorResponders;
    // affinity <-> sponsor
    private Map<String, CoordinatorSponsor> mCoordinatorSponsors;
    // affinity <-> executor
    private Map<String, CommandExecutor> mExecutorPool;
    // sponsor affinity <-> responder affinity
    private Map<String, Set<String>> mAffinityRelationShip;
    private PreTreatment mPreTreatment;

    @Override
    public void updatePropertiesInAction(String sponsorAffinity, String responderAffinity,
                                         LynxObject object, boolean notify) {
        if (mCoordinatorResponders != null
                && mExecutorPool != null
                && mAffinityRelationShip != null) {
            Set<String> responderAffinityList =
                    mAffinityRelationShip.get(sponsorAffinity);
            if (responderAffinityList == null) return;
            CommandExecutor executor = mExecutorPool.get(responderAffinity);
            Set<CoordinatorResponder> set = mCoordinatorResponders.get(responderAffinity);
            if (executor == null || set == null) return;
            for (Object o : object.getPropertyNames()) {
                String name = (String) o;
                Object value = object.getProperty(name);
                if (value instanceof String) {
                    executor.updateProperty(name, (String) value);
                } else if (value instanceof Boolean) {
                    executor.updateProperty(name, (boolean) value);
                } else {
                    executor.updateProperty(name, Double.valueOf(String.valueOf(value)));
                }
            }
            if (notify) {
                // Notify responder
                for (CoordinatorResponder responder : set) {
                    responder.coordinatorTreatment().onPropertiesUpdated(executor);
                }
                // Notify sponsor which is a responder
                CoordinatorSponsor sponsor = mCoordinatorSponsors.get(sponsorAffinity);
                if (sponsor != null && sponsor instanceof CoordinatorResponder) {
                    Treatment sponsorTreatment = ((CoordinatorResponder) sponsor).coordinatorTreatment();
                    if (sponsorTreatment != null) {
                        sponsorTreatment.onPropertiesUpdated(executor);
                    }
                }
            }
        }
    }

    @Override
    public void addExecutableAction(String sponsorAffinity, String responderAffinity, String executable) {
        if (mAffinityRelationShip == null) {
            mAffinityRelationShip = new HashMap<>();
        }
        if (mExecutorPool == null) {
            mExecutorPool = new HashMap<>();
        }
        Set<String> responderAffinityList = mAffinityRelationShip.get(sponsorAffinity);
        if (responderAffinityList == null) {
            responderAffinityList = new HashSet<>();
            mAffinityRelationShip.put(sponsorAffinity, responderAffinityList);
        }
        responderAffinityList.add(responderAffinity);

        CommandExecutor executor = mExecutorPool.get(responderAffinity);
        if (executor == null) {
            executor = new CommandExecutor(executable);
            mExecutorPool.put(responderAffinity, executor);
        } else {
            executor.update(executable);
        }

        if (mPreTreatment == null) {
            mPreTreatment = new PreTreatment();
        }
        // Init responder
        if (mCoordinatorResponders != null) {
            Set<CoordinatorResponder> set = mCoordinatorResponders.get(responderAffinity);
            if (set != null) {
                for (CoordinatorResponder responder : set) {
                    responder.coordinatorTreatment().init(executor);
                }
            }
        }
        // Init sponsor which is a responder
        if (mCoordinatorSponsors != null) {
            CoordinatorSponsor sponsor = mCoordinatorSponsors.get(sponsorAffinity);
            if (sponsor != null && sponsor instanceof CoordinatorResponder) {
                Treatment sponsorTreatment = ((CoordinatorResponder) sponsor).coordinatorTreatment();
                if (sponsorTreatment != null) {
                    sponsorTreatment.init(executor);
                }
            }
        }

    }

    @Override
    public void removeExecutableAction(String sponsorAffinity, String responderAffinity) {
        if (mAffinityRelationShip == null || mExecutorPool == null) {
            return;
        }
        Set<String> responderAffinityList = mAffinityRelationShip.get(sponsorAffinity);
        if (responderAffinityList == null) {
            return;
        }
        mAffinityRelationShip.remove(responderAffinityList);
        mExecutorPool.remove(responderAffinity);
    }

    @Override
    public void removeAllExecutableAction() {
        mAffinityRelationShip.clear();
        mExecutorPool.clear();
    }

    @Override
    public void addCoordinatorResponder(CoordinatorResponder responder) {
        if (mCoordinatorResponders == null) {
            mCoordinatorResponders = new HashMap<>();
        }
        Set<CoordinatorResponder> set = mCoordinatorResponders.get(responder.coordinatorAffinity());
        if (set == null) {
            set = new HashSet<>();
            mCoordinatorResponders.put(responder.coordinatorAffinity(), set);
        }
        set.add(responder);
        // Init responder
        if (mExecutorPool != null) {
            CommandExecutor executor = mExecutorPool.get(responder.coordinatorAffinity());
            if (executor != null) {
                responder.coordinatorTreatment().init(executor);
            }
        }
    }

    @Override
    public void removeCoordinatorResponder(CoordinatorResponder responder) {
        if (mCoordinatorResponders != null && responder != null) {
            Set<CoordinatorResponder> set = mCoordinatorResponders.get(responder.coordinatorAffinity());
            if (set != null) {
                set.remove(responder);
            }
        }
    }

    @Override
    public void addCoordinatorSponsor(CoordinatorSponsor sponsor) {
        if (mCoordinatorSponsors == null) {
            mCoordinatorSponsors = new HashMap<>();
        }
        mCoordinatorSponsors.put(sponsor.coordinatorAffinity(), sponsor);

        // Init sponsor which is a responder
        if (mAffinityRelationShip == null) return;
        Set<String> responderAffinityList =
                mAffinityRelationShip.get(sponsor.coordinatorAffinity());
        Treatment sponsorTreatment = null;
        if (sponsor instanceof CoordinatorResponder) {
            sponsorTreatment = ((CoordinatorResponder) sponsor).coordinatorTreatment();
        }
        if (responderAffinityList == null || sponsorTreatment == null) return;
        for (String responderAffinity : responderAffinityList) {
            CommandExecutor executor = mExecutorPool.get(responderAffinity);
            if (executor != null) sponsorTreatment.init(executor);
        }
    }

    @Override
    public void removeCoordinatorSponsor(CoordinatorSponsor sponsor) {
        if (mCoordinatorSponsors != null) {
            mCoordinatorSponsors.remove(sponsor.coordinatorAffinity());
        }
    }

    @Override
    public boolean dispatchNestedAction(String type, CoordinatorSponsor sponsor, Object... params) {
        boolean consumed = false;
        if (mCoordinatorResponders != null
                && mExecutorPool != null
                && mAffinityRelationShip != null) {
            Set<String> responderAffinityList =
                    mAffinityRelationShip.get(sponsor.coordinatorAffinity());
            if (responderAffinityList == null) return consumed;

            Treatment sponsorTreatment = null;
            if (sponsor instanceof CoordinatorResponder) {
                sponsorTreatment = ((CoordinatorResponder) sponsor).coordinatorTreatment();
            }

            for (String responderAffinity : responderAffinityList) {
                CommandExecutor executor = mExecutorPool.get(responderAffinity);
                Set<CoordinatorResponder> set = mCoordinatorResponders.get(responderAffinity);
                if (executor == null || set == null) continue;
                // First call onDispatch
                consumed = mPreTreatment.dispatchAction(type, executor,
                        sponsor.coordinatorTag(), params);
                if (sponsorTreatment != null) {
                    sponsorTreatment.onNestedAction(type, executor, params);
                }
                for (CoordinatorResponder responder : set) {
                    responder.coordinatorTreatment().onNestedAction(type, executor, params);
                }
            }
        }
        return consumed;
    }
}
