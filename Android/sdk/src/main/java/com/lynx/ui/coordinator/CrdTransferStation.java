// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.coordinator;

import com.lynx.core.base.LynxMap;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

public class CrdTransferStation implements TransferStation {
    // TransferStation
    // affinity <-> responders
    private Map<String, Set<CrdResponder>> mCoordinatorResponders;
    // affinity <-> sponsor
    private Map<String, CrdSponsor> mCoordinatorSponsors;
    // affinity <-> executor
    private Map<String, CrdCommandExecutor> mExecutorPool;
    // sponsor affinity <-> responder affinity
    private Map<String, Set<String>> mAffinityRelationShip;
    private CrdPreTreatment mPreTreatment;

    @Override
    public void updatePropertiesInAction(String sponsorAffinity, String responderAffinity,
                                         LynxMap object, boolean notify) {
        if (mCoordinatorResponders != null
                && mExecutorPool != null
                && mAffinityRelationShip != null) {
            Set<String> responderAffinityList =
                    mAffinityRelationShip.get(sponsorAffinity);
            if (responderAffinityList == null) return;
            CrdCommandExecutor executor = mExecutorPool.get(responderAffinity);
            Set<CrdResponder> set = mCoordinatorResponders.get(responderAffinity);
            if (executor == null || set == null) return;
            for (Object o : object.getKeySet()) {
                String name = (String) o;
                Object value = object.get(name);
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
                for (CrdResponder responder : set) {
                    responder.coordinatorTreatment().onPropertiesUpdated(executor);
                }
                // Notify sponsor which is a responder
                CrdSponsor sponsor = mCoordinatorSponsors.get(sponsorAffinity);
                if (sponsor != null && sponsor instanceof CrdResponder) {
                    CrdTreatment sponsorTreatment = ((CrdResponder) sponsor).coordinatorTreatment();
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

        CrdCommandExecutor executor = mExecutorPool.get(responderAffinity);
        if (executor == null) {
            executor = new CrdCommandExecutor(executable);
            mExecutorPool.put(responderAffinity, executor);
        } else {
            executor.update(executable);
        }

        if (mPreTreatment == null) {
            mPreTreatment = new CrdPreTreatment();
        }
        // Init responder
        if (mCoordinatorResponders != null) {
            Set<CrdResponder> set = mCoordinatorResponders.get(responderAffinity);
            if (set != null) {
                for (CrdResponder responder : set) {
                    responder.coordinatorTreatment().init(executor);
                }
            }
        }
        // Init sponsor which is a responder
        if (mCoordinatorSponsors != null) {
            CrdSponsor sponsor = mCoordinatorSponsors.get(sponsorAffinity);
            if (sponsor != null && sponsor instanceof CrdResponder) {
                CrdTreatment sponsorTreatment = ((CrdResponder) sponsor).coordinatorTreatment();
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
        responderAffinityList.remove(responderAffinity);
        CrdCommandExecutor executor = mExecutorPool.get(responderAffinity);
        executor.stop();
        mExecutorPool.remove(responderAffinity);
    }

    @Override
    public void removeAllExecutableAction() {
        if (mAffinityRelationShip != null) {
            mAffinityRelationShip.clear();
        }
        if (mExecutorPool != null) {
            for(CrdCommandExecutor  executor : mExecutorPool.values()) {
                executor.stop();
            }
            mExecutorPool.clear();
        }
    }

    @Override
    public void addCoordinatorResponder(CrdResponder responder) {
        if (mCoordinatorResponders == null) {
            mCoordinatorResponders = new HashMap<>();
        }
        Set<CrdResponder> set = mCoordinatorResponders.get(responder.coordinatorAffinity());
        if (set == null) {
            set = new HashSet<>();
            mCoordinatorResponders.put(responder.coordinatorAffinity(), set);
        }
        set.add(responder);
        // Init responder
        if (mExecutorPool != null) {
            CrdCommandExecutor executor = mExecutorPool.get(responder.coordinatorAffinity());
            if (executor != null) {
                responder.coordinatorTreatment().init(executor);
            }
        }
    }

    @Override
    public void removeCoordinatorResponder(CrdResponder responder) {
        if (mCoordinatorResponders != null && responder != null) {
            Set<CrdResponder> set = mCoordinatorResponders.get(responder.coordinatorAffinity());
            if (set != null) {
                set.remove(responder);
            }
        }
    }

    @Override
    public void addCoordinatorSponsor(CrdSponsor sponsor) {
        if (mCoordinatorSponsors == null) {
            mCoordinatorSponsors = new HashMap<>();
        }
        if (mCoordinatorSponsors.containsKey(sponsor.coordinatorAffinity())) return;
        mCoordinatorSponsors.put(sponsor.coordinatorAffinity(), sponsor);

        // Init sponsor which is a responder
        if (mAffinityRelationShip == null) return;
        Set<String> responderAffinityList =
                mAffinityRelationShip.get(sponsor.coordinatorAffinity());
        CrdTreatment sponsorTreatment = null;
        if (sponsor instanceof CrdResponder) {
            sponsorTreatment = ((CrdResponder) sponsor).coordinatorTreatment();
        }
        if (responderAffinityList == null || sponsorTreatment == null) return;
        for (String responderAffinity : responderAffinityList) {
            CrdCommandExecutor executor = mExecutorPool.get(responderAffinity);
            if (executor != null) sponsorTreatment.init(executor);
        }
    }

    @Override
    public void removeCoordinatorSponsor(CrdSponsor sponsor) {
        if (mCoordinatorSponsors != null) {
            mCoordinatorSponsors.remove(sponsor.coordinatorAffinity());
        }
    }

    @Override
    public boolean dispatchNestedAction(String type, CrdSponsor sponsor, Object... params) {
        boolean consumed = false;
        if (mCoordinatorResponders != null
                && mExecutorPool != null
                && mAffinityRelationShip != null) {
            Set<String> responderAffinityList =
                    mAffinityRelationShip.get(sponsor.coordinatorAffinity());
            if (responderAffinityList == null) return consumed;

            CrdTreatment sponsorTreatment = null;
            if (sponsor instanceof CrdResponder) {
                sponsorTreatment = ((CrdResponder) sponsor).coordinatorTreatment();
            }

            for (String responderAffinity : responderAffinityList) {
                CrdCommandExecutor executor = mExecutorPool.get(responderAffinity);
                Set<CrdResponder> set = mCoordinatorResponders.get(responderAffinity);
                if (executor == null || set == null) continue;
                // First call onDispatch
                consumed = mPreTreatment.dispatchAction(type, executor,
                        sponsor.coordinatorTag(), params);
                if (sponsorTreatment != null) {
                    sponsorTreatment.onNestedAction(type, executor, params);
                }
                for (CrdResponder responder : set) {
                    responder.coordinatorTreatment().onNestedAction(type, executor, params);
                }
            }
        }
        return consumed;
    }
}
