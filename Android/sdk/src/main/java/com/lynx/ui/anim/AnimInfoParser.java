// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.anim;

import android.support.annotation.NonNull;

import com.lynx.core.base.LynxArray;
import com.lynx.core.base.LynxMap;
import com.lynx.utils.PixelUtil;

import java.util.ArrayList;
import java.util.List;

public class AnimInfoParser {

    public static List<AnimInfo> parseKeyFrames(LynxArray keyframes) {
        return KeyFrame.create(keyframes);
    }

    public static AnimInfo.Option parseOption(LynxMap options) {
        return Options.create(options);
    }

    private static class KeyFrame {

        private static final String TRANSFORM = "transform";
        private static final String TRANSLATE_X = "translateX";
        private static final String TRANSLATE_Y = "translateY";
        private static final String TRANSLATE_Z = "translateZ";
        private static final String SCALE_X = "scaleX";
        private static final String SCALE_Y = "scaleY";
        private static final String SCALE_Z = "scaleZ";
        private static final String ROTATE_X = "rotateX";
        private static final String ROTATE_Y = "rotateY";
        private static final String ROTATE_Z = "rotateZ";
        private static final String SKEW_X = "skewX";
        private static final String SKEW_Y = "skewY";
        private static final String OPACITY = "opacity";
        private static final String PERSPECTIVE = "perspective";
        private static final String OFFSET = "offset";
        private static final String TRANSFORM_ORIGIN = "transformOrigin";

        public static List<AnimInfo> create(@NonNull LynxArray array) {
            List<AnimInfo> infoList = new ArrayList<>();
            for (int i = 0; i < array.length(); i++) {
                if (array.get(i) instanceof LynxMap) {
                    AnimInfo temp = create((LynxMap) array.get(i));
                    infoList.add(temp);
                } else throw new RuntimeException("Item in array should be a LynxObject!");
            }

            return infoList;
        }

        public static AnimInfo create(LynxMap keyframe) {
            AnimInfo info = new AnimInfo();

            info.option = new AnimInfo.Option();
            info.option.offset = Cast.toFloat(keyframe.get(OFFSET), 0);

            info.opacity = Cast.toFloat(keyframe.get(OPACITY), 1);

            Object transformOrigin = keyframe.get(TRANSFORM_ORIGIN);
            if (transformOrigin != null) {
                info.pivotX = Cast.toFloat(((LynxArray) transformOrigin).get(0), 0.5F);
                info.pivotY = Cast.toFloat(((LynxArray) transformOrigin).get(1), 0.5F);
            }

            Object easing = keyframe.get("easing");
            if (easing != null) {
                info.option.interpolator = TimingFunction.transform(
                        Cast.toString(easing, TimingFunction.EASING_LINEAR));
            }

            if (keyframe.get(TRANSFORM) == null
                    || !(keyframe.get(TRANSFORM) instanceof LynxMap)) {
                return info;
            }
            LynxMap transform = (LynxMap) keyframe.get(TRANSFORM);
            extract(transform, info);
            return info;
        }

        private static void extract(LynxMap transform, AnimInfo info) {

            info.translateX = (float) PixelUtil.dpToPx(
                    Cast.toFloat(transform.get(TRANSLATE_X), 0));
            info.translateY = (float) PixelUtil.dpToPx(
                    Cast.toFloat(transform.get(TRANSLATE_Y), 0));
            info.translateZ = (float) PixelUtil.dpToPx(
                    Cast.toFloat(transform.get(TRANSLATE_Z), 0));

            info.rotateX = Cast.toFloat(transform.get(ROTATE_X), 0);
            info.rotateY = Cast.toFloat(transform.get(ROTATE_Y), 0);
            info.rotateZ = Cast.toFloat(transform.get(ROTATE_Z), 0);

            info.scaleX = Cast.toFloat(transform.get(SCALE_X), 1);
            info.scaleY = Cast.toFloat(transform.get(SCALE_Y), 1);
            info.scaleZ = Cast.toFloat(transform.get(SCALE_Z), 1);

            info.skewX = Cast.toFloat(transform.get(SKEW_X), 0);
            info.skewY = Cast.toFloat(transform.get(SKEW_Y), 0);

            info.perspective = (float) PixelUtil.dpToPx(
                    Cast.toFloat(transform.get(PERSPECTIVE), Integer.MAX_VALUE));
        }
    }

    private static class Options {

        public final static String DIRECTION_NORMAL = "normal";
        public final static String DIRECTION_REVERSE = "reverse";
        public final static String DIRECTION_ALTERNATE = "alternate";
        public final static String DIRECTION_ALTERNATE_REVERSE = "alternate-reverse";

        public final static String FILL_BACKWARDS = "backwards";
        public final static String FILL_FORWARDS = "forwards";
        public final static String FILL_BOTH = "both";

        public static AnimInfo.Option create(LynxMap object) {
            AnimInfo.Option option = new AnimInfo.Option();
            option.id = Cast.toString(object.get("id"), "");
            option.duration = Cast.toInt(object.get("duration"), 0);
            option.delay = Cast.toInt(object.get("delay"), 0);
            option.endDelay = Cast.toInt(object.get("endDelay"), 0);
            // direction
            String direction = Cast.toString(object.get("direction"), DIRECTION_NORMAL);
            switch (direction) {
                case DIRECTION_ALTERNATE:
                    option.repeatMode = AnimInfo.Option.REVERSE;
                    option.direction = AnimInfo.Option.FORWARDS;
                    break;
                case DIRECTION_ALTERNATE_REVERSE:
                    option.repeatMode = AnimInfo.Option.REVERSE;
                    option.direction = AnimInfo.Option.BACKWARDS;
                    break;
                case DIRECTION_NORMAL:
                    option.repeatMode = AnimInfo.Option.RESTART;
                    option.direction = AnimInfo.Option.FORWARDS;
                    break;
                case DIRECTION_REVERSE:
                    option.repeatMode = AnimInfo.Option.RESTART;
                    option.direction = AnimInfo.Option.BACKWARDS;
                    break;
                default: break;
            }
            // interpolator
            String easing = Cast.toString(object.get("easing"), TimingFunction.EASING_LINEAR);
            option.interpolator = TimingFunction.transform(easing);
            // fill
            String fill = Cast.toString(object.get("fill"), "");
            switch (fill) {
                case FILL_BACKWARDS:
                    option.fillBefore = true;
                    break;
                case FILL_FORWARDS:
                    option.fillAfter = true;
                    break;
                case FILL_BOTH:
                    option.fillAfter = true;
                    option.fillBefore = true;
                    break;
                default: break;
            }
            // iterations
            Object iterationsObject = object.get("iterations");
            if (iterationsObject instanceof Double
                    && (Double)iterationsObject == Double.POSITIVE_INFINITY) {
                option.iterations = AnimInfo.Option.INFINITY;
            } else {
                option.iterations = Cast.toInt(iterationsObject, 1);
            }
            return option;
        }

    }

    private static class Cast {

        public static float toFloat(Object value, float defaultValue) {
            if (value instanceof Integer) {
                return (int) value;
            } else if (value instanceof Double) {
                return (float) (double) value;
            } else if (value instanceof Float) {
                return (float) value;
            }
            return defaultValue;
        }

        public static int toInt(Object value, int defaultValue) {
            if (value instanceof Integer) {
                return (int) value;
            } else if (value instanceof Double) {
                return (int) (double) value;
            } else if (value instanceof Float) {
                return (int) value;
            }
            return defaultValue;
        }

        public static boolean toBoolean(Object value, boolean defaultValue) {
            if (value instanceof Boolean) {
                return (Boolean) value;
            }
            return defaultValue;
        }

        public static String toString(Object value, String defaultValue) {
            if (value instanceof String) {
                return (String) value;
            }
            return defaultValue;
        }
    }
}
