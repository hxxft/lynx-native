// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.base;

public class Style {


    public final static int CSSTEXT_ALIGN_LEFT = 0;
    public final static int CSSTEXT_ALIGN_RIGHT = 1;
    public final static int CSSTEXT_ALIGN_CENTER = 2;
    public final static int CSSTEXT_TEXTDECORATION_LINETHROUGH = 3;
    public final static int CSSTEXT_TEXTDECORATION_NONE = 4;
    public final static int CSSTEXT_FONT_WEIGHT_NORMAL = 5;
    public final static int CSSTEXT_FONT_WEIGHT_BOLD = 6;
    public final static int CSSTEXT_OVERFLOW_ELLIPSIS = 7;
    public final static int CSSTEXT_OVERFLOW_NONE = 8;
    public final static int CSSTEXT_WHITESPACE_NOWRAP = 9;
    public final static int CSSTEXT_WHITESPACE_NORMAL = 10;

    public final static int CSSIMAGE_OBJECT_FIT_FILL = 0;
    public final static int CSSIMAGE_OBJECT_FIT_CONTAIN = 1;
    public final static int CSSIMAGE_OBJECT_FIT_COVER = 2;

    public final static int CSS_DISPLAY_NONE = 0;
    public final static int CSS_DISPLAY_FLEX = 1;
    public final static int CSSFLEX_DIRECTION_COLUMN = 7;
    public final static int SSFLEX_DIRECTION_COLUMN_REVERS = 8;
    public final static int CSSFLEX_DIRECTION_ROW = 9;
    public final static int CSSFLEX_DIRECTION_ROW_REVERSE = 10;
    public final static int CSS_POSITION_RELATIVE =18;
    public final static int CSS_POSITION_ABSOLUTE = 19;
    public final static int CSS_POSITION_FIXED = 20;
    public final static int CSS_VISIBLE = 21;
    public final static int CSS_HIDDEN = 22;
    public final static int CSS_POINTER_EVENT_NONE = 23;
    public final static int CSS_POINTER_EVENT_AUTO = 24;


    public int mBackgroundColor;
    public int mBorderWidth;
    public int mBorderColor;
    public double mBorderRadius;
    public double mOpacity ;
    public int mFlexDirection;
    public int mDisplayType;
    public int mVisible;
    public int mPositionType;
    public int mPointerEvents;

    public int mWidth;
    public int mHeight;

    public int mZIndex;

    //for text
    public int mFontColor;
    public int mFontSize;
    public int mFontWeight;
    public int mTextOverflow;
    public int mWhiteSpace;
    public int mTextAlign;
    public int mTextDecoration;
    public int mLineHeight;

    //for image
    public int mObjectFit;

    public int mPaddingLeft;
    public int mPaddingRight;
    public int mPaddingTop;
    public int mPaddingBottom;

    //screen density
    static public double sDensity;

    public static void init(double density) {
        sDensity = density;
    }

    public Style() {
        mWidth     = Constants.UNDEFINED;
        mHeight    = Constants.UNDEFINED;

        mPaddingLeft = 0;
        mPaddingRight = 0;
        mPaddingTop = 0;
        mPaddingBottom = 0;

        mVisible = CSS_VISIBLE;
        mBackgroundColor = 0;  // Color.parse("#00000000");
        mFontColor = 0xFF000000;  // Color.parse("#FF000000");
        mBorderWidth = 0;
        mBorderColor = 0xFF000000;  // Color.parse("#FF000000");
        mBorderRadius = 0;  // CSS_UNDEFINED;
        mOpacity = 255;
        mPointerEvents = CSS_POINTER_EVENT_AUTO;

        mFlexDirection = CSSFLEX_DIRECTION_ROW;

        mPositionType = CSS_POSITION_RELATIVE;
        mDisplayType = CSS_DISPLAY_FLEX;

        mFontSize = (int)(14 * sDensity);
        mFontWeight = CSSTEXT_FONT_WEIGHT_NORMAL;

        mTextAlign = CSSTEXT_ALIGN_LEFT;
        mWhiteSpace = CSSTEXT_WHITESPACE_NORMAL;
        mTextOverflow = CSSTEXT_OVERFLOW_NONE;
        mTextDecoration = CSSTEXT_TEXTDECORATION_NONE;
        mLineHeight = Constants.UNDEFINED;

        mObjectFit = CSSIMAGE_OBJECT_FIT_COVER;
        mZIndex = 0;
    }
}
