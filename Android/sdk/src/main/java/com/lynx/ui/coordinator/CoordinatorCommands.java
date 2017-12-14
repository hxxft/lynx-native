// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.coordinator;

import android.text.TextUtils;

import java.util.HashMap;
import java.util.Map;

public class CoordinatorCommands {
    private final static String COMMAND_DELIMITER = "\\|";
    private final static String COMMAND_DEFINITION_SYMBOL = ":";
    private Map<String, String> mCommands;

    public CoordinatorCommands(String content) {
        parse(content);
    }

    private void parse(String content) {
        if (TextUtils.isEmpty(content)) return;
        String[] commandGroup = content.split(COMMAND_DELIMITER);
        if (commandGroup.length == 0) return;
        mCommands = new HashMap<>();
        for (String command : commandGroup) {
            String[] details = command.split(COMMAND_DEFINITION_SYMBOL);
            if (details.length != 2) continue;
            switch (details[0].trim()) {
                case CoordinatorTypes.SCROLL:
                    mCommands.put(CoordinatorTypes.SCROLL, details[1].trim());
                    break;
                case CoordinatorTypes.TOUCH:
                    mCommands.put(CoordinatorTypes.TOUCH, details[1].trim());
                    break;
                default: break;
            }
        }
    }

    public String getCommand(String type) {
        return mCommands == null ? null : mCommands.get(type);
    }
}
