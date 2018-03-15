// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.impl;

import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Set;

public class EventManager {
    private Map<String, EventModifier> mEvents = new HashMap<>();
    private Map<String, LinkedHashMap<String, EventModifier>> mOptionEvents = new HashMap<>();

    public EventModifier addEvent(String event) {

        EventModifier eventModifier = new EventModifier(event);
        mEvents.put(event, eventModifier);

        LinkedHashMap<String, EventModifier> eventModifierMap
                = mOptionEvents.get(eventModifier.getName());
        if (eventModifierMap == null) {
            eventModifierMap = new LinkedHashMap<>();
            mOptionEvents.put(eventModifier.getName(), eventModifierMap);
        }
        eventModifierMap.put(event, eventModifier);

        return eventModifier;
    }

    public EventModifier removeEvent(String event) {
        EventModifier modifier = new EventModifier(event);
        mEvents.remove(modifier.getName());

        Map<String, EventModifier> eventModifierMap = mOptionEvents.get(modifier.getName());
        if (eventModifierMap != null) {
            eventModifierMap.remove(event);
        }

        return modifier;
    }

    public boolean isAllowedToDispatch(String event) {
        if (mEvents.containsKey(event) || mOptionEvents.containsKey(event)) {
            return true;
        }
        return false;
    }

    public Set<String> getEventNameSet() {
        return mEvents.keySet();
    }

    public Set<String> getOptionEventNameSet() {
        return mOptionEvents.keySet();
    }

    public EventModifier getEvent(String eventName) {
        return mEvents.get(eventName);
    }

    public Map<String, EventModifier> getOptionEvent(String groupName) {
        return mOptionEvents.get(groupName);
    }
}
