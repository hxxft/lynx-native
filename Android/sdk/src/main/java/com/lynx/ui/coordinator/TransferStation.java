// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.coordinator;

public interface TransferStation {

    void addExecutableAction(String sponsorAffinity, String responderAffinity, String executable);
    void removeExecutableAction(String sponsorAffinity, String responderAffinity);
    void removeAllExecutableAction();

    void addCoordinatorResponder(CoordinatorResponder responder);
    void removeCoordinatorResponder(CoordinatorResponder responder);

    boolean dispatchNestedAction(String type, CoordinatorSponsor sponsor, Object... params);
}
