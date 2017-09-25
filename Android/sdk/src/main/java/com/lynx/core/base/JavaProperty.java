// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.base;

/* package */ class JavaProperty {
    private StringBuilder mProperties;
    private Class[] mClassesOfProperties;

    public JavaProperty(Class[] properties) {
        mClassesOfProperties = properties;
        mProperties = new StringBuilder();
        if (properties != null) {
            for (int i = 0; i < properties.length; i++) {
                mProperties.append(ParamsTransform.transform(properties[i]));
            }
        }
    }

    public JavaProperty(Class property) {
        if (property != null) {
            mProperties = new StringBuilder();
            mProperties.append(ParamsTransform.transform(property));
        }
    }

    /**
     * Check if the args is matched with this JavaProperty. If it matched,
     * it will replace the element in args with the transformed object.
     * @param args
     * @return
     */
    public boolean isMatchedWith(Object[] args) {
        if (args != null
                && mClassesOfProperties != null
                && args.length == mClassesOfProperties.length) {
            Object[] temp = new Object[args.length];
            for (int i = 0; i < args.length; ++i) {
                temp[i] = MatchRule.instance()
                        .isMatch(args[i], mClassesOfProperties[i]);
                if (temp[i] == null) {
                    return false;
                }
            }
            for (int i = 0; i < args.length; ++i) {
                args[i] = temp[i];
            }
            return true;
        } else {
            return false;
        }
    }

    public String getTransformTypes() {
        return mProperties.toString();
    }

    public int length() {
        if (mClassesOfProperties != null) {
            return mClassesOfProperties.length;
        }
        return 0;
    }

    public static String propertiesType(Object... properties) {
        if (properties != null) {
            return ParamsTransform.transform(properties);
        }
        return "";
    }

    interface IMatchRule {
        /**
         * If the arg is matched with wantedType,
         * the correct object will be return
         * @param arg
         * @param wantedType
         * @return the transformed object
         */
        Object isMatch(Object arg, String wantedType);
    }

    static class MatchRule {
        static MatchRule sInstance;
        IMatchRule mNumberMatchRule;
        IMatchRule mStringMatchRule;
        IMatchRule mArrayRMatchRule;

        public static MatchRule instance() {
            if (sInstance == null) {
                sInstance = new MatchRule();
            }
            return sInstance;
        }

        public MatchRule() {
            mNumberMatchRule = new NumberMatchRule();
            mStringMatchRule = new StringMatchRule();
            mArrayRMatchRule = new ArrayMatchRule();
        }

        public Object isMatch(Object arg, Class clazz) {
            String type = ParamsTransform.transform(clazz);
            switch (type) {
                case JNIType.INI_TYPE:
                case JNIType.SHORT_TYPE:
                case JNIType.LONG_TYPE:
                case JNIType.DOUBLE_TYPE:
                case JNIType.FLOAT_TYPE:
                    return mNumberMatchRule.isMatch(arg, type);
                case JNIType.CHAR_TYPE:
                case JNIType.STRING_TYPE:
                    return mStringMatchRule.isMatch(arg, type);
                default:
                    break;
            }
            if (type.startsWith(JNIType.ARRAY_FLAG)) {
                return mArrayRMatchRule.isMatch(arg, type);
            }
            return null;
        }

    }

    private static class NumberMatchRule implements IMatchRule {

        @Override
        public Object isMatch(Object arg, String wantedType) {
            switch (wantedType) {
                case JNIType.INI_TYPE:
                    return Utils.toIntObject(arg);
                case JNIType.SHORT_TYPE:
                    return Utils.toShortObject(arg);
                case JNIType.LONG_TYPE:
                    return Utils.toLongObject(arg);
                case JNIType.DOUBLE_TYPE:
                    return Utils.toDoubleObject(arg);
                case JNIType.FLOAT_TYPE:
                    return Utils.toFloatObject(arg);
                default:
                    break;
            }
            return null;
        }
    }

    private static class StringMatchRule implements IMatchRule {

        @Override
        public Object isMatch(Object arg, String wantedType) {
            if (arg instanceof String) {
                switch (wantedType) {
                    case JNIType.CHAR_TYPE:
                        if (((String) arg).length() >= 1) {
                            return ((String) arg).charAt(0);
                        }
                        break;
                    case JNIType.STRING_TYPE:
                        return arg;
                    default:
                        break;
                }
            }
            return null;
        }
    }

    private static class ArrayMatchRule implements IMatchRule {

        @Override
        public Object isMatch(Object arg, String wantedType) {
            if (arg instanceof LynxArray
                    && ((LynxArray) arg).length() > 0) {
                // Check if the type of element in array is equal
                Object element = ((LynxArray) arg).get(0);
                String elementType = ParamsTransform.transform(element);

                if (wantedType.endsWith(elementType)) {
                    return Utils.toTypeArray((LynxArray) arg, elementType);
                }
            }
            return null;
        }
    }
}
