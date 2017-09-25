// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.input;

import android.content.Context;
import android.os.Build;
import android.support.annotation.NonNull;
import android.text.Editable;
import android.text.InputType;
import android.text.TextWatcher;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.TextView;

import com.lynx.base.Style;
import com.lynx.core.base.LynxEvent;
import com.lynx.core.base.LynxObject;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;

public class LynxUIInput extends LynxUI<AndroidInput> {

    private static final String EVENT_INPUT = "input";
    private static final String EVENT_FOCUS = "focus";
    private static final String EVENT_BLUR = "blur";
    private static final String EVENT_CONFIRM = "confirm";
    private static final String ATTR_AUTO_FOCUS = "auto-focus";
    private static final String ATTR_PLACE_HOLDER = "placeholder";
    private static final String ATTR_FOCUS = "focus";
    private static final String ATTR_MAX_LENGTH = "maxlength";

    private static final String ATTR_CONFIRM_TYPE = "confirm-type";
    private static final String CONFIRM_TYPE_SEND = "send";
    private static final String CONFIRM_TYPE_SEARCH = "search";
    private static final String CONFIRM_TYPE_NEXT = "next";
    private static final String CONFIRM_TYPE_GO = "go";
    private static final String CONFIRM_TYPE_DONE = "done";

    private static final String ATTR_TYPE = "type";
    private static final String TYPE_TEXT = "text";
    private static final String TYPE_NUMBER = "number";
    private static final String TYPE_PASSWORD = "password";
    private static final String TYPE_DIGIT = "digit";
    private static final int DEFAULT_MAX_LENGTH = 140;

    private int mMaxLength = DEFAULT_MAX_LENGTH;

    public LynxUIInput(Context context, RenderObjectImpl impl) {
        super(context, impl);
    }

    @Override
    protected AndroidInput createView(final Context context) {
        AndroidInput editText = new AndroidInput(context);
        editText.setSingleLine();
        editText.setPadding(0, 0, 0, 0);
        return editText;
    }

    @Override
    public void setText(String text) {
        if (text != null) {
            mView.setText(text);
            mView.setSelection(text.length());
        }
    }

    @Override
    public void updateStyle(Style style) {
        super.updateStyle(style);
        if (style != null) {
            setTextAlign(style);
            setTextDecoration(style);
            mView.setTextSize(TypedValue.COMPLEX_UNIT_PX, style.mFontSize);
            mView.setTextColor(style.mFontColor);
            setTextStyle(style);
            setPadding(style);
        }
    }

    @Override
    public void setAttribute(String key, String value) {
        super.setAttribute(key, value);
        switch (key) {
            case ATTR_PLACE_HOLDER:
                setPlaceHolder(mRenderObjectImpl);
                break;
            case ATTR_FOCUS:
                requestFocus(mRenderObjectImpl);
                break;
            case ATTR_MAX_LENGTH:
                setMaxLength(mRenderObjectImpl);
                break;
            case ATTR_TYPE:
                setType(mRenderObjectImpl);
                break;
            case ATTR_AUTO_FOCUS:
                setAutoFocus(mRenderObjectImpl);
                break;
            case ATTR_CONFIRM_TYPE:
                setConfirmType(mRenderObjectImpl);
                break;
            default:
                break;
        }
    }

    protected void setPlaceHolder(RenderObjectImpl impl) {
        if (impl == null) {
            mView.setHint("");
        } else {
            mView.setHint(impl.getAttribute(ATTR_PLACE_HOLDER));
        }
    }

    protected void requestFocus(RenderObjectImpl impl) {
        if (impl == null) return;
        String focus = impl.getAttribute(ATTR_FOCUS);
        if (Boolean.valueOf(focus)) {
            mView.requestFocus();

            InputMethodManager imm = (InputMethodManager) mView.getContext()
                    .getSystemService(Context.INPUT_METHOD_SERVICE);
            imm.showSoftInput(mView, InputMethodManager.SHOW_IMPLICIT);
        }
    }

    protected void setMaxLength(RenderObjectImpl impl) {
        mMaxLength = Integer.valueOf(impl.getAttribute(ATTR_MAX_LENGTH));
        mView.setMaxEms(mMaxLength);
    }

    protected void setType(RenderObjectImpl impl) {
        if (impl == null) return;
        String type = impl.getAttribute(ATTR_TYPE);
        switch (type) {
            case TYPE_TEXT:
                mView.setInputType(InputType.TYPE_CLASS_TEXT);
                break;
            case TYPE_DIGIT:
                mView.setInputType(InputType.TYPE_CLASS_NUMBER);
                break;
            case TYPE_PASSWORD:
                mView.setInputType(InputType.TYPE_CLASS_TEXT
                        | InputType.TYPE_TEXT_VARIATION_PASSWORD);
                break;
            case TYPE_NUMBER:
                mView.setInputType(InputType.TYPE_CLASS_PHONE);
                break;
            default:
                break;
        }
    }

    protected void setAutoFocus(RenderObjectImpl impl) {
        if (Boolean.valueOf(impl.getAttribute(ATTR_AUTO_FOCUS))) {
            mView.setAutoFocus(true);
        } else {
            mView.setAutoFocus(false);
        }
    }

    protected void setTextAlign(@NonNull Style style) {
        switch (style.mTextAlign) {
            case Style.CSSTEXT_ALIGN_CENTER:
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
                    mView.setTextAlignment(View.TEXT_ALIGNMENT_CENTER);
                } else if (mView.getGravity() != Gravity.CENTER){
                    mView.setGravity(Gravity.CENTER);
                }
                break;
            case Style.CSSTEXT_ALIGN_LEFT:
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
                    mView.setTextAlignment(View.TEXT_ALIGNMENT_VIEW_START);
                } else if (mView.getGravity() != Gravity.START){
                    mView.setGravity(Gravity.START);
                }
                break;
            case Style.CSSTEXT_ALIGN_RIGHT:
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
                    mView.setTextAlignment(View.TEXT_ALIGNMENT_VIEW_END);
                } else if (mView.getGravity() != Gravity.END){
                    mView.setGravity(Gravity.END);
                }
                break;
            default:
                break;
        }
    }

    protected void setConfirmType(RenderObjectImpl impl) {
        switch (impl.getAttribute(ATTR_CONFIRM_TYPE)) {
            case CONFIRM_TYPE_DONE:
                mView.setImeOptions(EditorInfo.IME_ACTION_DONE);
                break;
            case CONFIRM_TYPE_GO:
                mView.setImeOptions(EditorInfo.IME_ACTION_GO);
                break;
            case CONFIRM_TYPE_NEXT:
                mView.setImeOptions(EditorInfo.IME_ACTION_NEXT);
                break;
            case CONFIRM_TYPE_SEARCH:
                mView.setImeOptions(EditorInfo.IME_ACTION_SEARCH);
                break;
            case CONFIRM_TYPE_SEND:
                mView.setImeOptions(EditorInfo.IME_ACTION_SEND);
                break;
            default:
                mView.setImeOptions(EditorInfo.IME_NULL);
                break;
        }
    }

    protected void setTextDecoration(@NonNull Style style) {
        switch (style.mTextDecoration) {
            case Style.CSSTEXT_TEXTDECORATION_LINETHROUGH:
                mView.getPaint().setStrikeThruText(true);
                break;
            case Style.CSSTEXT_TEXTDECORATION_NONE:
                mView.getPaint().setStrikeThruText(false);
                break;
            default:
                break;
        }
    }

    protected void setTextStyle(@NonNull Style style) {
        switch (style.mFontWeight) {
            case Style.CSSTEXT_FONT_WEIGHT_BOLD:
                mView.getPaint().setFakeBoldText(true);
                break;
            case Style.CSSTEXT_FONT_WEIGHT_NORMAL:
                mView.getPaint().setFakeBoldText(false);
                break;
            default:
                break;
        }
    }

    protected void setPadding(Style style) {
        mView.setPadding(style.mPaddingLeft, style.mPaddingTop,
                style.mPaddingRight, style.mPaddingBottom);
    }

    @Override
    public void addEventListener(String event) {
        super.addEventListener(event);
        switch (event) {
            case EVENT_INPUT:
                mView.addTextChangedListener(new InnerTextWatcher());
                break;
            case EVENT_BLUR:
            case EVENT_FOCUS:
                mView.setOnFocusChangeListener(new InnerOnFocusChangeListener());
                break;
            case EVENT_CONFIRM:
                mView.setOnEditorActionListener(new InnerOnEditorActionListener());
                break;
            default:
                break;
        }
    }

    @Override
    public void removeEventListener(String event) {
        super.removeEventListener(event);
        switch (event) {
            case EVENT_INPUT:
                break;
            case EVENT_BLUR:
                break;
            case EVENT_FOCUS:
                break;
            case EVENT_CONFIRM:
                break;
            default:
                break;
        }
    }

    private class InnerOnFocusChangeListener implements View.OnFocusChangeListener {

        @Override
        public void onFocusChange(View v, boolean hasFocus) {
            if (hasFocus) {
                postEvent(EVENT_FOCUS);
            } else {
                postEvent(EVENT_BLUR);
            }
        }
    }

    private class InnerTextWatcher implements TextWatcher {
        @Override
        public void beforeTextChanged(CharSequence s, int start, int count, int after) {
        }

        @Override
        public void onTextChanged(CharSequence s, int start, int before, int count) {
            LynxEvent event = new LynxEvent(EVENT_INPUT);
            LynxObject detail = new LynxObject();
            detail.setProperty("oldText", mView.getText().toString());
            detail.setProperty("newText", s.toString());
            detail.setProperty("start", start);
            detail.setProperty("count", count);
            event.setProperty("detail", detail);
            postEvent(EVENT_INPUT, event);
        }

        @Override
        public void afterTextChanged(Editable s) {
        }
    }

    private class InnerOnEditorActionListener implements TextView.OnEditorActionListener {

        @Override
        public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
            switch (actionId) {
                case EditorInfo.IME_ACTION_GO:
                case EditorInfo.IME_ACTION_SEARCH:
                case EditorInfo.IME_ACTION_SEND:
                case EditorInfo.IME_ACTION_NEXT:
                case EditorInfo.IME_ACTION_DONE:
                    postEvent(EVENT_CONFIRM);
                    return true;
                default:
                    break;
            }
            return false;
        }
    }

}
