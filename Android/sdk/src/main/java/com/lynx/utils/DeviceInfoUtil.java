// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.utils;

import android.os.Build;

public class DeviceInfoUtil {
    private static String sDefaultUserAgent;

    public static String getInfo() {
        if (sDefaultUserAgent == null) {
            buildDefaultInfo();
        }
        return sDefaultUserAgent;
    }

    private static void buildDefaultInfo() {
        if (sDefaultUserAgent == null) {
            String temp = "Linux; Android " + Build.VERSION.RELEASE + "; " +
                     Build.MODEL + " Build/" + Build.DISPLAY;

            // transfer chinese to unicode
            StringBuilder sb = new StringBuilder();
            for (int i = 0, length = temp.length(); i < length; i++) {
                char c = temp.charAt(i);
                if (c <= '\u001f' || c >= '\u007f') {
                    sb.append(String.format("\\u%04x", (int) c));
                } else {
                    sb.append(c);
                }
            }

            sDefaultUserAgent = sb.toString();
        }
    }
}
