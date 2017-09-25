// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.base;

public class Utils {

    public static Object toIntObject(Object arg) {
        String type = ParamsTransform.transform(arg);
        switch (type) {
            case JNIType.INI_TYPE:
                return arg;
            case JNIType.DOUBLE_TYPE:
                return (int) (double) arg;
            default:
                break;
        }
        return null;
    }

    public static Object toShortObject(Object arg) {
        String type = ParamsTransform.transform(arg);
        switch (type) {
            case JNIType.INI_TYPE:
                return (short) (int) arg;
            case JNIType.DOUBLE_TYPE:
                return (short) (double) arg;
            default:
                break;
        }
        return null;
    }

    public static Object toLongObject(Object arg) {
        String type = ParamsTransform.transform(arg);
        switch (type) {
            case JNIType.INI_TYPE:
                return (long) (int) arg;
            case JNIType.DOUBLE_TYPE:
                return (long) (double) arg;
            default:
                break;
        }
        return null;
    }

    public static Object toFloatObject(Object arg) {
        String type = ParamsTransform.transform(arg);
        switch (type) {
            case JNIType.INI_TYPE:
                return (float) (int) arg;
            case JNIType.DOUBLE_TYPE:
                return (float) (double) arg;
            default:
                break;
        }
        return null;
    }

    public static Object toDoubleObject(Object arg) {
        String type = ParamsTransform.transform(arg);
        switch (type) {
            case JNIType.INI_TYPE:
                return (double) (int) arg;
            case JNIType.DOUBLE_TYPE:
                return arg;
            default:
                break;
        }
        return null;
    }


    public static Object toTypeArray(LynxArray array, String type) {
        Object result = null;
        switch (type) {
            case JNIType.BOOLEAN_TYPE:
                result = toBooleanArray(array);
                break;
            case JNIType.SHORT_TYPE:
                result = toShortArray(array);
                break;
            case JNIType.INI_TYPE:
                result = toIntArray(array);
                break;
            case JNIType.LONG_TYPE:
                result = toLongArray(array);
                break;
            case JNIType.FLOAT_TYPE:
                result = toFloatArray(array);
                break;
            case JNIType.DOUBLE_TYPE:
                result = toDoubleArray(array);
                break;
            case JNIType.BYTE_TYPE:
                result = toByteArray(array);
                break;
            case JNIType.CHAR_TYPE:
                result = toCharArray(array);
                break;
            case JNIType.STRING_TYPE:
                result = toStringArray(array);
                break;
            default:
                break;
        }
        return result;
    }

    public static int[] toIntArray(LynxArray array) {
        int[] result = new int[array.length()];
        for (int i = 0; i < array.length(); ++i) {
            result[i] = (int) array.get(i);
        }
        return result;
    }

    public static short[] toShortArray(LynxArray array) {
        short[] result = new short[array.length()];
        for (int i = 0; i < array.length(); ++i) {
            result[i] = (short) array.get(i);
        }
        return result;
    }

    public static long[] toLongArray(LynxArray array) {
        long[] result = new long[array.length()];
        for (int i = 0; i < array.length(); ++i) {
            result[i] = (long) array.get(i);
        }
        return result;
    }

    public static double[] toDoubleArray(LynxArray array) {
        double[] result = new double[array.length()];
        for (int i = 0; i < array.length(); ++i) {
            result[i] = (double) array.get(i);
        }
        return result;
    }

    public static float[] toFloatArray(LynxArray array) {
        float[] result = new float[array.length()];
        for (int i = 0; i < array.length(); ++i) {
            result[i] = (float) array.get(i);
        }
        return result;
    }

    public static boolean[] toBooleanArray(LynxArray array) {
        boolean[] result = new boolean[array.length()];
        for (int i = 0; i < array.length(); ++i) {
            result[i] = (boolean) array.get(i);
        }
        return result;
    }

    public static char[] toCharArray(LynxArray array) {
        char[] result = new char[array.length()];
        for (int i = 0; i < array.length(); ++i) {
            result[i] = (char) array.get(i);
        }
        return result;
    }

    public static String[] toStringArray(LynxArray array) {
        String[] result = new String[array.length()];
        for (int i = 0; i < array.length(); ++i) {
            result[i] = (String) array.get(i);
        }
        return result;
    }

    public static byte[] toByteArray(LynxArray array) {
        byte[] result = new byte[array.length()];
        for (int i = 0; i < array.length(); ++i) {
            result[i] = (byte) array.get(i);
        }
        return result;
    }
}
