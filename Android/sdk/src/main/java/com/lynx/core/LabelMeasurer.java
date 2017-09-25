// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core;

import android.graphics.Paint;
import android.graphics.Typeface;
import android.os.Build;
import android.text.Layout;
import android.text.Spannable;
import android.text.SpannableStringBuilder;
import android.text.StaticLayout;
import android.text.TextPaint;
import android.text.TextUtils;
import android.text.style.AlignmentSpan;
import android.text.style.LineHeightSpan;
import android.text.style.StrikethroughSpan;
import android.text.style.StyleSpan;
import android.text.style.TextAppearanceSpan;

import com.lynx.base.CalledByNative;
import com.lynx.base.Constants;
import com.lynx.base.Size;
import com.lynx.base.Style;

import java.util.ArrayList;
import java.util.List;

public class LabelMeasurer {

    private static final String ELLIPSIS = "\u2026";
    private static StaticLayout sLayout;

    private final static String XIAO_MI = "xiaomi";

    private static TextPaint getTextPaint(){
        TextPaint textPaint = new TextPaint();
        textPaint.setFlags(TextPaint.ANTI_ALIAS_FLAG);

        String manufacturer = Build.MANUFACTURER.toLowerCase();
        if (TextUtils.equals(XIAO_MI, manufacturer)) {
            Typeface sansSerif = Typeface.SERIF;
            textPaint.setTypeface(sansSerif);
        }
        return textPaint;
    }

    public static Typeface getTypeface() {
        return getTextPaint().getTypeface();
    }

    @CalledByNative
    public static StaticLayout getTextLayout() {
        return sLayout;
    }

    @CalledByNative
    public static Size measureLabelSize(String text, Style style, int width,
                                        int widthMode, int height, int heightMode) {
        sLayout = null;
        if (text == null) {
            text = "";
        }

        int widthWanted = style.mWidth;
        int heightWanted = style.mHeight;

        TextPaint textPaint = getTextPaint();

        // Set text color
        textPaint.setColor(style.mFontColor);

        // Set text size
        textPaint.setTextSize(style.mFontSize);

        SpannableStringBuilder span = fromLabelNode(style, text);

        int desiredWidth;
        // If width is sure, give the one it wants
        if (!Constants.isUndefined(widthWanted)) {
            desiredWidth = widthWanted;
        } else {
            desiredWidth = (int) Math.ceil(Layout.getDesiredWidth(text, textPaint));
        }

        // Determine if it should be a single line
        boolean shouldBeSingleLine = style.mWhiteSpace == Style.CSSTEXT_WHITESPACE_NOWRAP;
        // If it should not be single line, use given width instead.
        if (!shouldBeSingleLine &&
                (widthMode == SizeDescriptor.EXACTLY ||
                        (widthMode == SizeDescriptor.AT_MOST && desiredWidth > width))) {
            desiredWidth = width;
        }

        // When text or style is changed we use a new StaticLayout
        sLayout = new StaticLayout(
                span,
                0,
                span.length(),
                textPaint,
                desiredWidth,
                Layout.Alignment.ALIGN_NORMAL,
                1,
                0,
                // Includepad should be false as we change line padding top and bottom
                // by fm.ascent through XLineHeightSpan. First line and last line will
                // be affected by this, text draw {@link Layout#drawText} refers to
                // three line  above/baseline/below(ltop/lbaseline/lbottom),
                // and {@link FontMetricsInt} has top/ascent/descent/bottom/leading,
                // when includepad = true, it will treat fm.top as above and fm.bottom as below,
                // Otherwise, it will treat fm.ascent as ablove and fm.descent as below.
                false);

        // As double will cast to int, there are some precision losing when line num calculation.
        // So use value from style to calculate.
        int lineCountWanted = Constants.UNDEFINED;
        int lineHeight = style.mLineHeight;
        if (!Constants.isUndefined(lineHeight) &&
                !Constants.isUndefined(heightWanted)) {
            lineCountWanted = (int) Math.ceil(ignoreSomePrecision(heightWanted / lineHeight));
        }
        // If it want a single line, just make it come true.
        if (shouldBeSingleLine) {
            lineCountWanted = 1;
        }

        // If layout height is bigger than the given height, or is a single line, and has
        // ellipsis style, then use layout that after ellipsis
        boolean doEllipsis =
                style.mTextOverflow == Style.CSSTEXT_OVERFLOW_ELLIPSIS

                        && (
                        // Situation of single line.
                        (shouldBeSingleLine &&
                                (sLayout.getLineCount() > 1 || sLayout.getWidth() > width))

                                ||
                                // Situation of final line not visible.
                                (sLayout.getHeight() - height > sLayout.getHeight() / sLayout.getLineCount())

                                ||
                                // Situation of concrete line height.
                                (lineCountWanted != Constants.UNDEFINED && lineCountWanted < sLayout.getLineCount())
                );


        // Do ellipsis
        if (doEllipsis) {
            // Default target line is the first line
            int targetLine = 0;
            if (sLayout.getHeight() > height) {
                if (lineCountWanted != Constants.UNDEFINED) {
                    targetLine = lineCountWanted - 1;
                } else {
                    targetLine = (int) Math.ceil(height /
                            (sLayout.getHeight() / sLayout.getLineCount())) - 1;
                }
            }
            int surplus = sLayout.getWidth() - width;
            // Keep the target line and above, remove other lines below
            span.delete(sLayout.getLineEnd(targetLine), text.length());

            // Insert ellipsis
            span.insert(span.length(), ELLIPSIS);
            int ellipsisIndex = span.length() - 1;
            float ellipsisSize = Layout.getDesiredWidth(span, ellipsisIndex,
                    ellipsisIndex + 1, textPaint);
            if (ellipsisSize > width) {
                // If there no space for a ellipsis, show nothing
                span.clear();
            } else {
                // Find those characters that keep space the same as or larger than ellipsis,
                // and delete them, just like a "replace" action.
                int targetToDeleteTo = ellipsisIndex - 1;
                int targetToDeleteFrom = targetToDeleteTo;
                float targetSizeShouldBeDelete = 0;
                float totalSizeShouldBeDelete = ellipsisSize + surplus;
                while (targetToDeleteFrom > 0) {
                    targetSizeShouldBeDelete += Layout.getDesiredWidth(span, targetToDeleteFrom,
                            targetToDeleteFrom + 1, textPaint);
                    if (targetSizeShouldBeDelete >= totalSizeShouldBeDelete) {
                        break;
                    }
                    targetToDeleteFrom--;
                }
                if (targetToDeleteFrom <= 0) {
                    span.clear();
                } else {
                    span.delete(targetToDeleteFrom, targetToDeleteTo + 1);
                }
            }
            // After do ellipsis, refresh layout
            desiredWidth = width;
            sLayout = new StaticLayout(
                    span,
                    0,
                    span.length(),
                    textPaint,
                    desiredWidth,
                    Layout.Alignment.ALIGN_NORMAL,
                    1,
                    0,
                    false);
        }

        Size measuredSize = new Size();

        // Set measured size
        measuredSize.mHeight = sLayout.getHeight();
        measuredSize.mWidth = sLayout.getWidth();

        return measuredSize;
    }

    private static SpannableStringBuilder fromLabelNode(Style style, String text) {
        SpannableStringBuilder sb = new SpannableStringBuilder();
        // TODO(5837930): Investigate whether it's worth optimizing this part and do it if so

        // The {@link SpannableStringBuilder} implementation require setSpan operation to be called
        // up-to-bottom, otherwise all the spannables that are withing the region for which one may set
        // a new spannable will be wiped out
        List<SetSpanOperation> ops = new ArrayList<>();
        buildSpannedFromTextCSSNode(style, text, sb, ops);

        // While setting the Spans on the final text, we also check whether any of them are images
        for (int i = ops.size() - 1; i >= 0; i--) {
            SetSpanOperation op = ops.get(i);
            op.execute(sb);
        }
        return sb;
    }

    private static void buildSpannedFromTextCSSNode(Style style, String text, SpannableStringBuilder sb, List<SetSpanOperation> ops) {
        int start = sb.length();
        if (text != null) {
            sb.append(text);
        }

        int end = sb.length();
        if (end >= start) {
            // Do not do this as it will slow down layout.draw(), do it by TextPaint instead.
            // You can do it with span while you want some effect like rich text.
            /*
            // Set text color
            ops.add(new SetSpanOperation(start, end, new ForegroundColorSpan(style.mColor)));
            // Set background color
            ops.add(new SetSpanOperation(start, end, new BackgroundColorSpan(style.mBackgroundColor)));
            // Set text decoration
            // Set text size
            if (!CSSFlexConstants.isUndefined(style.mFontSize)) {
                ops.add(new SetSpanOperation(start, end, new AbsoluteSizeSpan(PixelUtils.toPixelFromSP(style.mFontSize))));
            } else {
                ops.add(new SetSpanOperation(start, end, new AbsoluteSizeSpan(PixelUtils.toPixelFromSP(FONT_SIZE_SP))));
            }
            */

            // Set text font weight
            if (style.mFontWeight == Style.CSSTEXT_FONT_WEIGHT_BOLD) {
                ops.add(new SetSpanOperation(start, end, new StyleSpan(Typeface.BOLD)));
            }

            if (style.mTextDecoration == Style.CSSTEXT_TEXTDECORATION_LINETHROUGH) {
                ops.add(new SetSpanOperation(start, end, new StrikethroughSpan()));
            }
//            else if (style.mTextDecoration == Style.CSSTEXT_TEXTDECORATION_UNDERLINE) {
//                ops.add(new SetSpanOperation(start, end, new UnderlineSpan()));
//            }
            // Set text alignment
            if (style.mTextAlign == Style.CSSTEXT_ALIGN_LEFT) {
                ops.add(new SetSpanOperation(start, end, new AlignmentSpan.Standard(Layout.Alignment.ALIGN_NORMAL)));
            } else if (style.mTextAlign == Style.CSSTEXT_ALIGN_CENTER) {
                ops.add(new SetSpanOperation(start, end, new AlignmentSpan.Standard(Layout.Alignment.ALIGN_CENTER)));
            } else {
                ops.add(new SetSpanOperation(start, end, new AlignmentSpan.Standard(Layout.Alignment.ALIGN_OPPOSITE)));
            }
            // Set text line-height
            if (!Constants.isUndefined(style.mLineHeight)) {
                ops.add(new SetSpanOperation(start, end, new XLineHeightSpan(style.mLineHeight)));
            } else {
                ops.add(new SetSpanOperation(start, end, new XLineHeightSpan()));
            }
        }
    }

    private static class SetSpanOperation {
        protected int start, end;
        protected Object what;
        SetSpanOperation(int start, int end, Object what) {
            this.start = start;
            this.end = end;
            this.what = what;
        }
        public void execute(SpannableStringBuilder sb) {
            // All spans will automatically extend to the right of the text, but not the left - except
            // for spans that start at the beginning of the text.
            int spanFlags = Spannable.SPAN_EXCLUSIVE_INCLUSIVE;
            if (start == 0) {
                spanFlags = Spannable.SPAN_INCLUSIVE_INCLUSIVE;
            }
            sb.setSpan(what, start, end, spanFlags);
        }
    }

    private static class XLineHeightSpan implements LineHeightSpan {

        private int mLineHeight = Constants.UNDEFINED;

        XLineHeightSpan() {
        }

        XLineHeightSpan(int lineHeight) {
            mLineHeight = lineHeight;
        }

        @Override
        public void chooseHeight(CharSequence text, int start, int end, int spanstartv, int v, Paint.FontMetricsInt fm) {

            // Reduce the above and below position of the span for the same layout effect as web
            fm.ascent = fm.top + (fm.bottom - fm.descent);
            // Control line-height
            if (mLineHeight != Constants.UNDEFINED) {
                int ajust = (mLineHeight - (fm.descent - fm.ascent)) / 2;
                fm.descent += ajust;
                fm.ascent -=ajust;
            }
        }
    }

    private static double ignoreSomePrecision(double a) {
        a = (int) (a * 10);
        a = a / 10;
        return a;
    }
}
