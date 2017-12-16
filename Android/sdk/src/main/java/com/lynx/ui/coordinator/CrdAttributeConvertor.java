// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.coordinator;

import android.text.TextUtils;

public class CrdAttributeConvertor {

    public final static String ATTR_COORDINATOR_TAG = "coordinator-tag";
    public final static String ATTR_COORDINATOR_AFFINITY = "coordinator-affinity";
    public final static String ATTR_COORDINATOR_TYPE = "coordinator-type";

    private String mCoordinatorTag;
    private String mCoordinatorAffinity;
    private CrdTypes mCrdTypes;

    public void setCoordinatorAffinity(TransferStation transferStation, CrdObject object,
                                       String affinity) {
        if (mCoordinatorAffinity != null && mCoordinatorTag.equals(affinity)) {
            return;
        }
        CrdResponder responder = object instanceof CrdResponder ?
                (CrdResponder) object : null;
        CrdSponsor sponsor = object instanceof CrdSponsor ?
                (CrdSponsor) object : null;
        // First remove coordinator in TransferStation
        if (mCoordinatorAffinity != null && !mCoordinatorAffinity.equals(affinity)) {
            if (responder != null ) transferStation.removeCoordinatorResponder(responder);
            if (sponsor != null && mCrdTypes != null) {
                transferStation.removeCoordinatorSponsor(sponsor);
            }
        }
        // Update affinity
        mCoordinatorAffinity = affinity;
        // Re-add into TransferStation
        if (!TextUtils.isEmpty(affinity)) {
            if (responder != null ) transferStation.addCoordinatorResponder(responder);
            if (sponsor != null && mCrdTypes != null) {
                transferStation.addCoordinatorSponsor(sponsor);
            }
        }
    }

    public void setCoordinatorType(TransferStation transferStation, CrdSponsor sponsor,
                                      String type) {
        if (mCrdTypes != null
                && type != null
                && mCrdTypes.getRawContent() != null
                && mCrdTypes.getRawContent().equals(type)) {
            return;
        }
        if (!TextUtils.isEmpty(type)) {
            mCrdTypes = new CrdTypes(type);
        } else {
            mCrdTypes = null;
        }
        if (TextUtils.isEmpty(mCoordinatorAffinity)) return;
        if (mCrdTypes != null) {
            transferStation.addCoordinatorSponsor(sponsor);
        } else {
            transferStation.removeCoordinatorSponsor(sponsor);
        }
    }

    // Reset tag will trigger responder to init
    public void setCoordinatorTag(TransferStation transferStation, CrdObject object,
                                  String tag) {
        if (mCoordinatorTag != null && mCoordinatorTag.equals(tag)) {
            return;
        }
        mCoordinatorTag = tag;
        if (object instanceof CrdResponder) {
            CrdResponder responder = (CrdResponder) object;
            responder.coordinatorTreatment().reset();
            transferStation.addCoordinatorResponder(responder);
        }
    }

    public CrdTypes getCoordinatorTypes() {
        return mCrdTypes;
    }

    public String getCoordinatorTag() {
        return mCoordinatorTag == null? "" : mCoordinatorTag;
    }

    public String getCoordinatorAffinity() {
        return mCoordinatorAffinity;
    }
}
