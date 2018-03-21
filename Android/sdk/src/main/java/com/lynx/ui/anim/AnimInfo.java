// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.anim;

import android.view.animation.Animation;
import android.view.animation.Interpolator;

public class AnimInfo {

    /**
     * The perspective CSS property determines the distance between the z=0 plane and
     * the user in order to give a 3D-positioned element some perspective. Each 3D
     * element with z>0 becomes larger; each 3D-element with z<0 becomes smaller. The
     * strength of the effect is determined by the value of this property.
     */
    public float perspective;

    /**
     * Defines a transformation that translate a view on the 3D plane.
     */
    public float translateX;
    public float translateY;
    public float translateZ;

    /**
     * Defines a transformation that rotates a view on the 3D plane.
     * The unit of value should be degree.
     */
    public float rotateX;
    public float rotateY;
    public float rotateZ;

    /**
     * Defines a transformation that resizes a view on the 3D plane.
     */
    public float scaleX = 1.0f;
    public float scaleY = 1.0f;
    public float scaleZ = 1.0f;

    /**
     * Defines a transformation that skews a view on the 2D plane.
     * The unit of value should be degree.
     */
    public float skewX;
    public float skewY;

    /**
     * Specifies the transparency of view, Any values outside the range 0.0
     * (fully transparent) to 1.0 (fully opaque) will be clamped to this range.
     */
    public float opacity = 1.0f;

    /**
     * Sets the origin for an view's transformations.
     */
    public float pivotX = 0.5f;
    public float pivotY = 0.5f;

    public Option option;

    public static class Option {

        public static final String EVENT_FINISH = "finish";
        public static final String EVENT_CANCEL = "cancel";

        public static final int TYPE_2D = 0;
        public static final int TYPE_3D = 1;
        public static final int RESTART = Animation.RESTART;
        public static final int REVERSE = Animation.REVERSE;
        public static final int FORWARDS = 0;
        public static final int BACKWARDS = 1;

        public static final int INFINITY = Animation.INFINITE;

        /**
         * The identity of the Animation.
         */
        public String id;

        /**
         * The animation type. Defaults to 3D
         */
        public int type = TYPE_3D;

        /**
         * The number of milliseconds each iteration of the animation takes to complete.
         * Defaults to 0. Although this is technically optional, keep in mind that your
         * animation will not run if this value is 0.
         */
        public int duration;

        /**
         * The keyframe offset of the keyframe specified as a number between 0.0 and 1.0 inclusive.
         * This only responses to KeyFrame Animation
         */
        public float offset;

        /**
         * The number of milliseconds to delay the start of the animation. Defaults to 0.
         */
        public int delay;

        /**
         * Dictates Whether the animation runs forwards {@link #FORWARDS} or
         * backwards {@link #BACKWARDS}.
         */
        public int direction;

        /**
         * Dictates Whether the animation should switch direction after each iteration or not.
         * see {@link #RESTART} and {@link #REVERSE}
         */
        public int repeatMode;

        /**
         * The rate of the animation's change over time.
         */
        public Interpolator interpolator;

        /**
         * The number of milliseconds to delay after the end of an animation.
         * This is primarily of use when sequencing animations based on the end
         * time of another animation. Defaults to 0.
         */
        public int endDelay;

        /**
         * Dictates whether the animation's effects should be retained after the animation
         * has completed playing forwards. Defaults to false.
         */
        public boolean fillAfter;

        /**
         * Dictates whether the animation's effects should be reflected by the element(s)
         * prior to playing backwards. Defaults to false.
         */
        public boolean fillBefore;

        /**
         * The number of times the animation should repeat. Defaults to 1, and can also
         * take a value of {@link #INFINITY} to make it repeat for as long as the element exists.
         * 0 means no animation.
         */
        public int iterations = 1;
    }
}
