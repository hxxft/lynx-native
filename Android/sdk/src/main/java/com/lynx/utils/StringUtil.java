// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.utils;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

public class StringUtil {

    public static String convertToString(InputStream in) {
        return convertToString(in, null);
    }

    public static String convertToString(InputStream in, String encoding) {
        if (in == null) {
            return "";
        }
        if (encoding == null) {
            encoding = "UTF8";
        }
        try {
            BufferedReader r = new BufferedReader(new InputStreamReader(in, encoding));
            StringBuilder total = new StringBuilder();
            String line;
            while ((line = r.readLine()) != null) {
                total.append(line).append('\n');
            }
            return total.toString();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return "";
    }
}
