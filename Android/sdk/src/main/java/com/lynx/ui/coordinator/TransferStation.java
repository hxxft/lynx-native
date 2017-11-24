// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.coordinator;

import com.lynx.core.base.LynxObject;

public interface TransferStation {

    void updatePropertiesInAction(String sponsorAffinity, String responderAffinity,
                                  LynxObject object, boolean notify);
    void addExecutableAction(String sponsorAffinity, String responderAffinity, String executable);
    void removeExecutableAction(String sponsorAffinity, String responderAffinity);
    void removeAllExecutableAction();

    void addCoordinatorResponder(CoordinatorResponder responder);
    void removeCoordinatorResponder(CoordinatorResponder responder);

    void addCoordinatorSponsor(CoordinatorSponsor sponsor);
    void removeCoordinatorSponsor(CoordinatorSponsor sponsor);

    boolean dispatchNestedAction(String type, CoordinatorSponsor sponsor, Object... params);
}
