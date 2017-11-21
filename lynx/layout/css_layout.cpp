// Copyright 2017 The Lynx Authors. All rights reserved.

#include <vector>
#include "render/render_object.h"
#include "layout/css_type.h"
#include "layout/css_layout.h"
#include "render/render_tree_host.h"

using namespace base;
using namespace std;

namespace lynx {


    Size CSSStaticLayout::Measure(LayoutObject* renderer, int width, int height) {

        CSSStyle* style = renderer->GetStyle();

        Size size = measureInner(renderer, Size::Descriptor::GetSize(width), Size::Descriptor::GetSize(height));

        int w = Size::Descriptor::GetMode(width) == Size::Descriptor::EXACTLY ? Size::Descriptor::GetSize(width) : style->ClampWidth(size.width_);
        int h = Size::Descriptor::GetMode(height) == Size::Descriptor::EXACTLY ? Size::Descriptor::GetSize(height) : style->ClampHeight(size.height_);

        size.Update(w, h);
        return size;
    }

    Size CSSStaticLayout::measureInner(LayoutObject* renderer, int width, int height) {

        Size measuredSize;
        CSSStyle* style = renderer->GetStyle();

        int w = width;
        int h = height;

        int availableWidth = w - style->padding_left_
                - style->padding_right_ - style->border_width_ * 2;
        int availableHeight = h - style->padding_top_
                - style->padding_bottom_ - style->border_width_ * 2;

        if (style->flex_direction_ == CSSFLEX_DIRECTION_ROW) {
            measuredSize = measureRow(renderer, availableWidth, availableHeight);
        } else {
            //measure
            measuredSize = measureColumn(renderer, availableWidth, availableHeight);
        }

        w = measuredSize.width_ + style->padding_left_
                + style->padding_right_ + style->border_width_ * 2;
        h = measuredSize.height_ + style->padding_top_
                + style->padding_bottom_ + style->border_width_ * 2;


        measuredSize.Update(w, h);
        return measuredSize;
    }

    bool CSSStaticLayout::measureSpecially(LayoutObject* renderer, int width, int height) {
        CSSStyle* childStyle = renderer->GetStyle();
        if(childStyle->visible_ == CSS_HIDDEN
                || childStyle->css_display_type_ == CSS_DISPLAY_NONE) {
            return true;
        }
        if(childStyle->css_position_type_ == CSS_POSITION_ABSOLUTE) {
            measureAbsolute(renderer, width, height);
            return true;
        }
        if(childStyle->css_position_type_ == CSS_POSITION_FIXED) {
            measureFixed(renderer);
            return true;
        }
        return false;
    }

    Size CSSStaticLayout::measureRowOneLine(LayoutObject* renderer, int width, int height, int start, int end) {
        Size measuredSize = Size(0 ,0);
        int calcWidth = 0;
        int calcHeight = 0;

        int totalFlex = 0;
        // 对于row排版，且是no_wrap的操作，孩子想要多宽就给多宽
        int residualWidth = width;

        //计算子view中无flex属性的view size，并且计算出剩下含有flex属性的子view可使用的宽度
        for (int i = start; i <= end; i++) {

            int measureWidth;

            LayoutObject* child = (LayoutObject*)renderer->Find(i);
            CSSStyle* childStyle = child->GetStyle();

            if(measureSpecially(child, width, height)) {
                continue;
            }

            if(childStyle->flex_ > 0) {
                totalFlex += childStyle->flex_;
                calcWidth += childStyle->margin_left_ + childStyle->margin_right_;
                continue;
            }

            if(!CSS_IS_UNDEFINED(childStyle->width_)){
                measureWidth = Size::Descriptor::Make(childStyle->ClampWidth(), Size::Descriptor::EXACTLY);
            }else{
                measureWidth = Size::Descriptor::Make(residualWidth - childStyle->margin_left_ - childStyle->margin_right_, Size::Descriptor::AT_MOST);
            }

            Size childSize = child->Measure(measureWidth, Size::Descriptor::Make(height, Size::Descriptor::AT_MOST));

            calcWidth += childSize.width_ + childStyle->margin_left_ + childStyle->margin_right_;
            calcHeight = childSize.height_;

            //residualWidth = width - calcWidth;


            int childIteheight_ = calcHeight + childStyle->margin_bottom_ + childStyle->margin_top_;
            measuredSize.height_ = measuredSize.height_ > childIteheight_ ? measuredSize.height_ : childIteheight_;
        }

        if(calcWidth == 0 && totalFlex > 0)
            residualWidth = renderer->GetStyle()->ClampWidth(width) - calcWidth;
        else
            residualWidth = renderer->GetStyle()->ClampWidth(calcWidth) - calcWidth;

        for(int i = start; i <= end; i++) {

            LayoutObject* child = (LayoutObject*)renderer->Find(i);
            CSSStyle* childStyle = child->GetStyle();

            if(childStyle->visible_ == CSS_HIDDEN
                    || childStyle->css_display_type_ == CSS_DISPLAY_NONE) {
                continue;
            }
            if(childStyle->css_position_type_ == CSS_POSITION_ABSOLUTE
                    || childStyle->css_position_type_ == CSS_POSITION_FIXED) {
                continue;
            }

            if(childStyle->flex_ <= 0) continue;

            //根据flex属性重新计算子view宽度
            int recalcWidth = residualWidth * childStyle->flex_/totalFlex;

            int measureWidth = recalcWidth == 0 ? Size::Descriptor::Make(width, Size::Descriptor::AT_MOST) : Size::Descriptor::Make(recalcWidth, Size::Descriptor::EXACTLY);
            //测量子view的宽高/
            Size childSize = child->Measure(measureWidth,
                    Size::Descriptor::Make(height, Size::Descriptor::AT_MOST));


            calcWidth += childSize.width_;
            calcHeight = childSize.height_;


            int childIteheight_ = calcHeight + childStyle->margin_bottom_ + childStyle->margin_top_;
            measuredSize.height_ = measuredSize.height_ > childIteheight_ ? measuredSize.height_ : childIteheight_;

        }
        measuredSize.width_ = calcWidth;
        //measuredSize.height_ = calcHeight;
        return measuredSize;
    }

    Size CSSStaticLayout::measureRowWrap(LayoutObject* renderer, int width, int height) {

        int currentCalcWidth = 0;
        int calcWidth = 0;
        int calcHeight = 0;
        int start = 0;

        Size measuredSize = Size(renderer->GetStyle()->width_ ,renderer->GetStyle()->height_);

        for(int index = 0, childViewCount = renderer->GetChildCount(); index < childViewCount;) {

            LayoutObject* child = (LayoutObject*)renderer->Find(index);
            CSSStyle* childStyle = child->GetStyle();

            if(measureSpecially(child, width, height)) {
                continue;
            }

            int measureWidth = width - childStyle->margin_left_ - childStyle->margin_right_;
            Size childSize = Size(0, 0);

            if(childStyle->flex_ == 0) {

                childSize = child->Measure(Size::Descriptor::Make(measureWidth, Size::Descriptor::AT_MOST),
                        Size::Descriptor::Make(height, Size::Descriptor::AT_MOST));
            }

            currentCalcWidth = currentCalcWidth + childSize.width_ + childStyle->margin_left_ + childStyle->margin_right_;
            if(currentCalcWidth <= width) {
                calcWidth = currentCalcWidth;
                if(index == childViewCount - 1) {
                    Size rowSize = measureRowOneLine(renderer, width, height, start, index);
                    calcWidth = rowSize.width_ > calcWidth ? rowSize.width_ : calcWidth;
                    calcHeight += rowSize.height_;
                }
                index++;
            }

            // 当前行宽度占满，准备往下排时，或者是已经排到了最后一行的最后一个元素
            else {

                int end = index;
                if(start != index) {
                    end -= 1;
                }

                Size rowSize = measureRowOneLine(renderer, width, height, start, end);

                calcWidth = rowSize.width_ > calcWidth ? rowSize.width_ : calcWidth;
                calcHeight += rowSize.height_;
                if(start == index) {
                    index += 1;
                }
                start = index;
                currentCalcWidth = 0;
            }
        }
        measuredSize.width_ = calcWidth;
        measuredSize.height_ = calcHeight;
        return measuredSize;
    }

    Size CSSStaticLayout::measureRow(LayoutObject* renderer, int width, int height) {

        CSSStyle* itemStyle = renderer->GetStyle();
        if (itemStyle->flex_wrap_ == CSSFLEX_NOWRAP) {
            return measureRowOneLine(renderer, width, height, 0, renderer->GetChildCount()-1);
        }else {
            //measure flex-wrap
            return measureRowWrap(renderer, width, height);
        }
    }

    Size CSSStaticLayout::measureColumnOneLine(LayoutObject* renderer, int width, int height, int start, int end) {
        Size measuredSize = Size(0 ,0);
        int calcWidth = 0;
        int calcHeight = 0;

        int totalFlex = 0;
        // 对于row排版，且是no_wrap的操作，孩子想要多宽就给多宽
        int residualHeight = height;

        //计算子view中无flex属性的view size，并且计算出剩下含有flex属性的子view可使用的宽度
        for (int i = start; i <= end; i++) {

            int measureHeight;

            LayoutObject* child = (LayoutObject*)renderer->Find(i);
            CSSStyle* childStyle = child->GetStyle();

            if(measureSpecially(child, width, height)) {
                continue;
            }

            if(childStyle->flex_ > 0) {
                totalFlex += childStyle->flex_;
                calcHeight += childStyle->margin_top_ + childStyle->margin_bottom_;
                continue;
            }

            if(!CSS_IS_UNDEFINED(childStyle->height_)){
                measureHeight = Size::Descriptor::Make(childStyle->ClampHeight(), Size::Descriptor::EXACTLY);
            }else{
                measureHeight = Size::Descriptor::Make(residualHeight - childStyle->margin_top_ - childStyle->margin_bottom_, Size::Descriptor::AT_MOST);
            }

            Size childSize = child->Measure(Size::Descriptor::Make(width, Size::Descriptor::AT_MOST), measureHeight);

            calcWidth = childSize.width_;
            calcHeight += childSize.height_ + childStyle->margin_top_ + childStyle->margin_bottom_;

            int childItewidth_ = calcWidth + childStyle->margin_left_ + childStyle->margin_right_;
            measuredSize.width_ = measuredSize.width_ > childItewidth_ ? measuredSize.width_ : childItewidth_;
        }

        if(calcHeight == 0 && totalFlex > 0)
            residualHeight = renderer->GetStyle()->ClampHeight(height) - calcHeight;
        else
            residualHeight = renderer->GetStyle()->ClampHeight(calcHeight) - calcHeight;

        for(int i = start; i <= end; i++) {

            LayoutObject* child = (LayoutObject*)renderer->Find(i);
            CSSStyle* childStyle = child->GetStyle();

            if(childStyle->visible_ == CSS_HIDDEN
                    || childStyle->css_display_type_ == CSS_DISPLAY_NONE) {
                continue;
            }
            if(childStyle->css_position_type_ == CSS_POSITION_ABSOLUTE
                    || childStyle->css_position_type_ == CSS_POSITION_FIXED) {
                continue;
            }

            if(childStyle->flex_ <= 0) continue;

            //根据flex属性重新计算子view宽度
            int recalcHeight = residualHeight * childStyle->flex_/totalFlex;

            //测量子view的宽高/
            int measureHeight = recalcHeight == 0 ? Size::Descriptor::Make(height, Size::Descriptor::AT_MOST) : Size::Descriptor::Make(recalcHeight, Size::Descriptor::EXACTLY);
            Size childSize = child->Measure(Size::Descriptor::Make(width, Size::Descriptor::AT_MOST),
                    measureHeight);


            calcWidth = childSize.width_;
            calcHeight += childSize.height_;


            int childItewidth_ = calcWidth + childStyle->margin_left_ + childStyle->margin_right_;
            measuredSize.width_ = measuredSize.width_ > childItewidth_ ? measuredSize.width_ : childItewidth_;

        }
        measuredSize.height_ = calcHeight;
        return measuredSize;
    }

    Size CSSStaticLayout::measureColumnWrap(LayoutObject* renderer, int width, int height) {

        int currentCalcHeight = 0;
        int calcWidth = 0;
        int calcHeight = 0;
        int start = 0;

        Size measuredSize = Size(renderer->GetStyle()->width_ ,renderer->GetStyle()->height_);

        for(int index = 0, childViewCount = renderer->GetChildCount(); index < childViewCount;) {

            LayoutObject* child = (LayoutObject*)renderer->Find(index);
            CSSStyle* childStyle = child->GetStyle();

            if(measureSpecially(child, width, height)) {
                continue;
            }

            int measureHeight = height - childStyle->margin_top_ - childStyle->margin_bottom_;
            Size childSize = Size(0, 0);

            if(childStyle->flex_ == 0) {

                childSize = child->Measure(Size::Descriptor::Make(width, Size::Descriptor::AT_MOST),
                        Size::Descriptor::Make(measureHeight, Size::Descriptor::AT_MOST));
            }

            currentCalcHeight = currentCalcHeight + childSize.height_ + childStyle->margin_top_ + childStyle->margin_bottom_;
            if(currentCalcHeight <= height) {
                calcHeight = currentCalcHeight;
                if(index == childViewCount - 1) {
                    Size columnSize = measureColumnOneLine(renderer, width, height, start, index);

                    calcWidth += columnSize.width_;
                    calcHeight = columnSize.height_ > calcHeight ? columnSize.height_ : calcHeight;
                }
                index++;
            }

            // 当前行宽度占满，准备往下排时，或者是已经排到了最后一行的最后一个元素
            else  {

                int end = index;
                //start != index 意味着当前宽/高超过可拍宽/高，因此一行测量需要减去最后一个元素
                if(start != index) {
                    end -= 1;
                }

                Size columnSize = measureColumnOneLine(renderer, width, height, start, end);

                calcWidth += columnSize.width_;
                calcHeight = columnSize.height_ > calcHeight ? columnSize.height_ : calcHeight;

                //start == index 表示一行只有一个元素需要排版
                if(start == index) {
                    index += 1;
                }
                start = index;
            }
        }
        measuredSize.width_ = calcWidth;
        measuredSize.height_ = calcHeight;
        return measuredSize;
    }

    Size CSSStaticLayout::measureColumn(LayoutObject* renderer, int width, int height) {

        CSSStyle* itemStyle = renderer->GetStyle();
        if (itemStyle->flex_wrap_ == CSSFLEX_NOWRAP) {
            return measureColumnOneLine(renderer, width, height, 0, renderer->GetChildCount()-1);
        }else {
            //measure flex-wrap
            return measureColumnWrap(renderer, width, height);
        }
    }

    void CSSStaticLayout::Layout(LayoutObject* renderer, int width, int height) {
        CSSStyle* itemStyle = renderer->GetStyle();
        Size itemSize  = renderer->GetMeasuredSize();
        // 如果是隐藏的view，之后的view都不进行排版，需要重新measure
        if (itemStyle->css_display_type_ == CSS_DISPLAY_NONE) {
            return;
        }

        if(itemStyle->flex_direction_ == CSSFLEX_DIRECTION_ROW) {
            layoutRow(renderer, width, height);
        }else if(itemStyle->flex_direction_ == CSSFLEX_DIRECTION_COLUMN){
            layoutColumn(renderer, width, height);
        }
    }

    void CSSStaticLayout::layoutRowWrap(LayoutObject* renderer, int width, int height) {
        CSSStyle* itemStyle = renderer->GetStyle();
        Size itemSize = renderer->GetMeasuredSize();

        int availableWidth = width - itemStyle->padding_left_
                             - itemStyle->padding_right_ - itemStyle->border_width_*2;
        int availableHeight = height - itemStyle->padding_top_
                              - itemStyle->padding_bottom_ - itemStyle->border_width_ * 2;

        int currentRowWithoutAbsoluteCount = 0;
        int totalUseWidthWithoutAbsolute = 0;

        int start = 0;
        int totalUsedHeight = 0;

        int currentLineHeight = 0;
        bool isLineFeed = false;

        for (int index = 0, childViewCount = renderer->GetChildCount(); index < childViewCount; index++) {

            LayoutObject* child = (LayoutObject*)renderer->Find(index);
            CSSStyle* childStyle = child->GetStyle();

            bool shouldChildPass = false;
            int oldTotalUseWidthWithoutAbsolute = totalUseWidthWithoutAbsolute;
            if(childStyle->visible_ == CSS_HIDDEN ||
                    childStyle->css_display_type_ == CSS_DISPLAY_NONE) {
                child->Layout(0, 0, 0, 0);
                shouldChildPass = true;
            }
            if(childStyle->css_position_type_ == CSS_POSITION_ABSOLUTE) {
                layoutAbsolute(renderer, child, width, height);
                shouldChildPass = true;
            }
            if(childStyle->css_position_type_ == CSS_POSITION_FIXED) {
                layoutFixed(renderer, child);
                shouldChildPass = true;
            }

            if (!shouldChildPass) {

                currentRowWithoutAbsoluteCount++;

                totalUseWidthWithoutAbsolute += child->GetMeasuredSize().width_ + childStyle->margin_left_ + childStyle->margin_right_;

            }

            if(totalUseWidthWithoutAbsolute <= availableWidth) {
                int childHeight = child->GetMeasuredSize().height_ + childStyle->margin_top_ + childStyle->margin_bottom_;
                currentLineHeight = currentLineHeight > childHeight ? currentLineHeight : childHeight;
            }

            // 遍历到最后一个孩子的时候，不可以跳过layout previous child的操作，否则之前的孩子不能排版
            bool isLastView = (index == childViewCount - 1);
            if (shouldChildPass && !isLastView) {
                continue;
            }

            if(totalUseWidthWithoutAbsolute > availableWidth || isLastView) {

                if(totalUseWidthWithoutAbsolute > availableWidth && currentRowWithoutAbsoluteCount > 1) {
                    isLineFeed = true;
                    index--;
                    currentRowWithoutAbsoluteCount--;
                    totalUseWidthWithoutAbsolute = oldTotalUseWidthWithoutAbsolute;
                }else if(isLastView && !isLineFeed) {
                    currentLineHeight = availableHeight;
                }

                int adjustWidthStart = 0;
                int adjustWidthInterval = 0;
                if(currentRowWithoutAbsoluteCount > 0) {
                    if(itemStyle->flex_justify_content_ == CSSFLEX_JUSTIFY_FLEX_START) {
                        //no action
                    }else if(itemStyle->flex_justify_content_ == CSSFLEX_JUSTIFY_FLEX_END) {
                        adjustWidthStart = availableWidth - totalUseWidthWithoutAbsolute;
                    }else if(itemStyle->flex_justify_content_ == CSSFLEX_JUSTIFY_FLEX_CENTER) {
                        adjustWidthStart = (availableWidth - totalUseWidthWithoutAbsolute) / 2;
                    }else if(totalUseWidthWithoutAbsolute <= availableWidth && itemStyle->flex_justify_content_ == CSSFLEX_JUSTIFY_SPACE_BETWEEN) {
                        if(currentRowWithoutAbsoluteCount > 1) {
                            adjustWidthInterval = round(((float)(availableWidth - totalUseWidthWithoutAbsolute)) /
                                    (currentRowWithoutAbsoluteCount-1));
                        }
                    }else if(totalUseWidthWithoutAbsolute <= availableWidth && itemStyle->flex_justify_content_ == CSSFLEX_JUSTIFY_SPACE_AROUND) {
                        float interval = ((float)(availableWidth - totalUseWidthWithoutAbsolute)) /
                                (currentRowWithoutAbsoluteCount*2);
                        adjustWidthStart = round(interval);
                        adjustWidthInterval = round(interval*2);
                    }
                }

                int maxHeight = 0;
                int childOriginX = itemStyle->padding_left_ + itemStyle->border_width_ + adjustWidthStart;
                int childOriginY = itemStyle->padding_top_ + itemStyle->border_width_  + totalUsedHeight;


                for (int i = start; i <= index; i++) {

                    LayoutObject* recalcChild = (LayoutObject*)renderer->Find(i);
                    CSSStyle* recalcChildStyle = recalcChild->GetStyle();

                    CSSStyleType align = itemStyle->flex_align_items_;

                    if(recalcChildStyle->visible_ == CSS_HIDDEN ||
                            recalcChildStyle->css_display_type_ == CSS_DISPLAY_NONE) {
                        recalcChild->Layout(0,0,0,0);
                        continue;
                    }
                    if(recalcChildStyle->css_position_type_ == CSS_POSITION_ABSOLUTE ||
                            recalcChildStyle->css_position_type_ == CSS_POSITION_FIXED) {
                        continue;
                    }

                    if(recalcChildStyle->flex_align_self_ != CSSFLEX_ALIGN_AUTO) {
                        align = recalcChildStyle->flex_align_self_;
                    }


                    int oldChildOriginY = childOriginY;

                    childOriginX += recalcChildStyle->margin_left_;
                    if(i > start)
                        childOriginX += adjustWidthInterval;
                    childOriginY += recalcChildStyle->margin_top_;

                    int adjustY = 0;
                    int adjustHeight = CSS_UNDEFINED;
                    if(align == CSSFLEX_ALIGN_FLEX_START) {
                        //do nothing
                    }else if(align == CSSFLEX_ALIGN_FLEX_END) {
                        adjustY = currentLineHeight - recalcChildStyle->margin_bottom_ - recalcChild->GetMeasuredSize().height_;
                    }else if(align == CSSFLEX_ALIGN_STRETCH) {
                        adjustHeight = currentLineHeight - recalcChildStyle->margin_top_ - recalcChildStyle->margin_bottom_;
                        adjustHeight = recalcChild->GetStyle()->ClampHeight(adjustHeight);
                    }else if(align == CSSFLEX_ALIGN_CENTER) {
                        adjustY = (currentLineHeight - recalcChild->GetMeasuredSize().height_)/2;
                    }

                    int l = childOriginX;
                    int t = childOriginY + adjustY;
                    int r = l + recalcChild->GetMeasuredSize().width_;
                    int b = CSS_IS_UNDEFINED(adjustHeight) ?
                            t + recalcChild->GetMeasuredSize().height_ : t + adjustHeight;

                    recalcChild->Layout(l,t,r,b);

                    childOriginX += recalcChild->GetMeasuredSize().width_ + recalcChildStyle->margin_right_;
                    childOriginY = oldChildOriginY;
                    int childIteheight_ = (b-t) + recalcChildStyle->margin_bottom_ + recalcChildStyle->margin_top_;
                    maxHeight = maxHeight < childIteheight_ ? childIteheight_ : maxHeight;
                }
                start = index + 1;
                totalUseWidthWithoutAbsolute = itemStyle->padding_left_ + itemStyle->padding_right_;
                currentRowWithoutAbsoluteCount = 0;
                totalUsedHeight += maxHeight;
                currentLineHeight = 0;
            }
        }
    }

    void CSSStaticLayout::layoutRow(LayoutObject* renderer, int width, int height) {
        CSSStyle* itemStyle = renderer->GetStyle();
        // 可用宽高为去除padding和border之后的宽高
        int availableWidth = width - itemStyle->padding_left_
                             - itemStyle->padding_right_ - itemStyle->border_width_*2;
        int availableHeight = height - itemStyle->padding_top_
                              - itemStyle->padding_bottom_ - itemStyle->border_width_*2;

        int currentRowWithoutAbsoluteCount = 0;
        int totalUseWidthWithoutAbsolute = 0;

        if(itemStyle->flex_wrap_ == CSSFLEX_NOWRAP) {
            for (int index = 0, childViewCount = renderer->GetChildCount(); index < childViewCount; index++) {

                LayoutObject* child = (LayoutObject*)renderer->Find(index);
                CSSStyle* childStyle = child->GetStyle();

                if(childStyle->visible_ == CSS_HIDDEN ||
                        childStyle->css_display_type_ == CSS_DISPLAY_NONE) {
                    child->Layout(0,0,0,0);
                    continue;
                }
                if(childStyle->css_position_type_ == CSS_POSITION_ABSOLUTE) {
                    layoutAbsolute(renderer, child, width, height);
                    continue;
                }
                if(childStyle->css_position_type_ == CSS_POSITION_FIXED) {
                    layoutFixed(renderer, child);
                    continue;
                }
                currentRowWithoutAbsoluteCount++;
                totalUseWidthWithoutAbsolute += child->GetMeasuredSize().width_
                        + child->GetStyle()->margin_left_ + child->GetStyle()->margin_right_;
            }

            int adjustWidthStart = 0;
            int adjustWidthInterval = 0;
            if(currentRowWithoutAbsoluteCount > 0) {
                if(itemStyle->flex_justify_content_ == CSSFLEX_JUSTIFY_FLEX_START) {
                    //no action
                }else if(itemStyle->flex_justify_content_ == CSSFLEX_JUSTIFY_FLEX_END) {
                    adjustWidthStart = availableWidth - totalUseWidthWithoutAbsolute;
                }else if(itemStyle->flex_justify_content_ == CSSFLEX_JUSTIFY_FLEX_CENTER) {
                    adjustWidthStart = (availableWidth - totalUseWidthWithoutAbsolute) / 2;
                }else if(totalUseWidthWithoutAbsolute <= availableWidth && itemStyle->flex_justify_content_ == CSSFLEX_JUSTIFY_SPACE_BETWEEN) {
                    if(currentRowWithoutAbsoluteCount > 1) {
                        adjustWidthInterval = round(((float)(availableWidth - totalUseWidthWithoutAbsolute)) /
                                (currentRowWithoutAbsoluteCount-1));
                    }
                }else if(totalUseWidthWithoutAbsolute <= availableWidth && itemStyle->flex_justify_content_ == CSSFLEX_JUSTIFY_SPACE_AROUND) {
                    float interval = ((float)(availableWidth - totalUseWidthWithoutAbsolute)) /
                            (currentRowWithoutAbsoluteCount*2);
                    adjustWidthStart += round(interval);
                    adjustWidthInterval = round(interval*2);
                }
            }

            // 防止有absolute的孩子导致第一个布局的孩子判断不准确
            int firstShowIndex = 0;
            int childOriginX = adjustWidthStart + itemStyle->padding_left_ + itemStyle->border_width_;
            int childOriginY = itemStyle->padding_top_ + itemStyle->border_width_;
            for (int i = 0, size = renderer->GetChildCount(); i < size; i++) {
                LayoutObject* child = (LayoutObject*)renderer->Find(i);
                CSSStyle* childStyle = child->GetStyle();

                CSSStyleType align = itemStyle->flex_align_items_;

                if(childStyle->visible_ == CSS_HIDDEN) {
                    child->Layout(0,0,0,0);
                    continue;
                }
                if(childStyle->css_position_type_ == CSS_POSITION_ABSOLUTE ||
                        childStyle->css_position_type_ == CSS_POSITION_FIXED ||
                        childStyle->css_display_type_ == CSS_DISPLAY_NONE) {
                    firstShowIndex++;
                    continue;
                }

                if(childStyle->flex_align_self_ != CSSFLEX_ALIGN_AUTO) {
                    align = childStyle->flex_align_self_;
                }

                int oldChildOriginY = childOriginY;

                childOriginX += childStyle->margin_left_;
                if(i > firstShowIndex)
                    childOriginX += adjustWidthInterval;
                childOriginY += childStyle->margin_top_;

                int adjustY = 0;
                int adjustHeight = CSS_UNDEFINED;
                if(align == CSSFLEX_ALIGN_FLEX_START) {
                    //do nothing
                }else if(align == CSSFLEX_ALIGN_FLEX_END) {
                    adjustY = availableHeight - childStyle->margin_bottom_ - child->GetMeasuredSize().height_;
                }else if(align == CSSFLEX_ALIGN_STRETCH) {
                    if (CSS_IS_UNDEFINED(childStyle->height_)) {
                        adjustHeight = availableHeight - childStyle->margin_top_ - childStyle->margin_bottom_;
                        adjustHeight = child->GetStyle()->ClampHeight(adjustHeight);
                    }
                }else if(align == CSSFLEX_ALIGN_CENTER) {
                    adjustY = (availableHeight - child->GetMeasuredSize().height_ )/2;
                }

                int l = childOriginX;
                int t = adjustY + childOriginY;
                int r = childOriginX + child->GetMeasuredSize().width_;
                int b = CSS_IS_UNDEFINED(adjustHeight) ?
                        childOriginY + child->GetMeasuredSize().height_ + adjustY
                        : childOriginY + adjustHeight;
                child->Layout(l,t,r,b);

                childOriginX += child->GetMeasuredSize().width_ + childStyle->margin_right_;
                childOriginY = oldChildOriginY;
            }
        }else {
            layoutRowWrap(renderer, width, height);
        }
    }

    void CSSStaticLayout::layoutColumnWrap(LayoutObject* renderer, int width, int height) {
        CSSStyle* itemStyle = renderer->GetStyle();

        int availableWidth = width - itemStyle->padding_left_
                - itemStyle->padding_right_ - itemStyle->border_width_*2;
        int availableHeight = height;

        int currentColumnWithoutAbsoluteCount = 0;
        int totalUseHeightWithoutAbsolute = itemStyle->padding_top_
                + itemStyle->padding_bottom_;


        int start = 0;
        int size = renderer->GetChildCount();
        int totalUsedWidth = 0;


        int currentLineWidth = 0;
        bool isLineFeed = false;

        for (int index = 0; index < size; index++) {
            LayoutObject* child = (LayoutObject*)renderer->Find(index);
            CSSStyle* childStyle = child->GetStyle();

            bool childShouldPass = false;
            int oldTotalUseHeightWithoutAbsolute = totalUseHeightWithoutAbsolute;
            if(childStyle->visible_ == CSS_HIDDEN ||
                    childStyle->css_display_type_ == CSS_DISPLAY_NONE) {
                child->Layout(0, 0, 0, 0);
                childShouldPass = true;
            }
            if(childStyle->css_position_type_ == CSS_POSITION_ABSOLUTE) {
                layoutAbsolute(renderer, child, width, height);
                childShouldPass = true;
            }
            if(childStyle->css_position_type_ == CSS_POSITION_FIXED) {
                layoutFixed(renderer, child);
                childShouldPass = true;
            }

            if (!childShouldPass) {
                currentColumnWithoutAbsoluteCount++;
                if(!CSS_IS_UNDEFINED(childStyle->height_)) {

                    totalUseHeightWithoutAbsolute += childStyle->height_ + childStyle->margin_top_ + childStyle->margin_bottom_;

                }else{

                    totalUseHeightWithoutAbsolute += child->GetMeasuredSize().height_ + childStyle->margin_top_ + childStyle->margin_bottom_;

                }
            }

            if(totalUseHeightWithoutAbsolute <= availableHeight) {
                int childWidth = child->GetMeasuredSize().width_ + childStyle->margin_left_ + childStyle->margin_right_;
                currentLineWidth = currentLineWidth > childWidth ? currentLineWidth : childWidth;
            }

            // 遍历到最后一个孩子的时候，不可以跳过layout previous child的操作，否则之前的孩子不能排版
            bool isLastView = (index == size - 1);
            if (childShouldPass && !isLastView) {
                continue;
            }

            if(totalUseHeightWithoutAbsolute > availableHeight || isLastView) {

                if(totalUseHeightWithoutAbsolute > availableHeight && currentColumnWithoutAbsoluteCount > 1) {
                    isLineFeed = true;
                    index--;
                    currentColumnWithoutAbsoluteCount--;
                    totalUseHeightWithoutAbsolute = oldTotalUseHeightWithoutAbsolute;
                }else if(isLastView && !isLineFeed) {
                    currentLineWidth = availableWidth;
                }

                int adjustHeightStart = 0;
                int adjustHeightInterval = 0;
                if(currentColumnWithoutAbsoluteCount > 0) {
                    if(itemStyle->flex_justify_content_ == CSSFLEX_JUSTIFY_FLEX_START) {
                        //no action
                    }else if(itemStyle->flex_justify_content_ == CSSFLEX_JUSTIFY_FLEX_END) {
                        adjustHeightStart = availableHeight - totalUseHeightWithoutAbsolute;
                    }else if(itemStyle->flex_justify_content_ == CSSFLEX_JUSTIFY_FLEX_CENTER) {
                        adjustHeightStart = (availableHeight - totalUseHeightWithoutAbsolute) / 2;
                    }else if(totalUseHeightWithoutAbsolute <= availableWidth && itemStyle->flex_justify_content_ == CSSFLEX_JUSTIFY_SPACE_BETWEEN) {
                        if(currentColumnWithoutAbsoluteCount > 1) {
                            adjustHeightInterval = round(((float)(availableHeight - totalUseHeightWithoutAbsolute)) /
                                    (currentColumnWithoutAbsoluteCount-1));
                        }
                    }else if(totalUseHeightWithoutAbsolute <= availableWidth && itemStyle->flex_justify_content_ == CSSFLEX_JUSTIFY_SPACE_AROUND) {
                        float interval = ((float)(availableHeight - totalUseHeightWithoutAbsolute)) /
                                (currentColumnWithoutAbsoluteCount*2);
                        adjustHeightStart = round(interval);
                        adjustHeightInterval = round(interval*2);
                    }
                }

                int maxWidth = 0;
                int childOriginX = itemStyle->padding_left_ + itemStyle->border_width_ + totalUsedWidth;
                int childOriginY = itemStyle->padding_top_  + itemStyle->border_width_  + adjustHeightStart;
                for (int i = start; i <= index; i++) {
                    LayoutObject* recalcChild = (LayoutObject*)renderer->Find(i);
                    CSSStyle* recalcChildStyle = recalcChild->GetStyle();

                    CSSStyleType align = itemStyle->flex_align_items_;

                    if(recalcChildStyle->visible_ == CSS_HIDDEN ||
                            recalcChildStyle->css_display_type_ == CSS_DISPLAY_NONE) {
                        recalcChild->Layout(0,0,0,0);
                        continue;
                    }
                    if(recalcChildStyle->css_position_type_ == CSS_POSITION_ABSOLUTE ||
                            recalcChildStyle->css_position_type_ == CSS_POSITION_FIXED) {
                        continue;
                    }

                    if(recalcChildStyle->flex_align_self_ != CSSFLEX_ALIGN_AUTO) {
                        align = recalcChildStyle->flex_align_self_;
                    }

                    int oldChildOriginX = childOriginX;

                    childOriginX += recalcChildStyle->margin_left_;
                    if(i >= start)
                        childOriginY += recalcChildStyle->margin_top_ + adjustHeightInterval;

                    int adjustX = 0;
                    int adjustWidth = CSS_UNDEFINED;
                    if(align == CSSFLEX_ALIGN_FLEX_START) {
                        //do nothing
                    }else if(align == CSSFLEX_ALIGN_FLEX_END) {
                        adjustX = currentLineWidth - recalcChildStyle->margin_right_ - recalcChild->GetMeasuredSize().width_;
                    }else if(align == CSSFLEX_ALIGN_STRETCH) {
                        adjustWidth = currentLineWidth - recalcChildStyle->margin_right_ - recalcChildStyle->margin_left_;
                        adjustWidth = recalcChild->GetStyle()->ClampWidth(adjustWidth);
                    }else if(align == CSSFLEX_ALIGN_CENTER) {
                        adjustX = (currentLineWidth - recalcChildStyle->margin_right_ -
                                recalcChildStyle->margin_left_ - recalcChild->GetMeasuredSize().width_)/2;
                    }


                    int l = childOriginX + adjustX;
                    int t = childOriginY;
                    int r = CSS_IS_UNDEFINED(adjustWidth) ?
                            l + recalcChild->GetMeasuredSize().width_ : l + adjustWidth;
                    int b = t + recalcChild->GetMeasuredSize().height_;

                    recalcChild->Layout(l, t, r, b);

                    childOriginX = oldChildOriginX;
                    childOriginY += recalcChild->GetMeasuredSize().height_ + recalcChildStyle->margin_bottom_;
                    int childItewidth_ = (r-l) + recalcChildStyle->margin_left_ + recalcChildStyle->margin_right_;
                    maxWidth = maxWidth < childItewidth_ ? childItewidth_ : maxWidth;
                }
                start = index + 1;
                totalUseHeightWithoutAbsolute = itemStyle->padding_top_
                        + itemStyle->padding_bottom_;
                currentColumnWithoutAbsoluteCount = 0;
                totalUsedWidth += maxWidth;
                currentLineWidth = 0;
            }
        }
    }

    void CSSStaticLayout::layoutColumn(LayoutObject* renderer, int width, int height) {
        CSSStyle* itemStyle = renderer->GetStyle();

        int availableWidth = width - itemStyle->padding_left_
                - itemStyle->padding_right_ - itemStyle->border_width_*2;
        int availableHeight = height;

        int currentColumnWithoutAbsoluteCount = 0;
        int totalUseHeightWithoutAbsolute = itemStyle->padding_top_
                + itemStyle->padding_bottom_;

        if(itemStyle->flex_wrap_ == CSSFLEX_NOWRAP) {
            for (int i = 0, size = renderer->GetChildCount(); i < size; i++) {
                LayoutObject* child = (LayoutObject*)renderer->Find(i);
                CSSStyle* childStyle = child->GetStyle();
                if(childStyle->visible_ == CSS_HIDDEN ||
                        childStyle->css_display_type_ == CSS_DISPLAY_NONE) {
                    child->Layout(0, 0, 0, 0);
                    continue;
                }
                if(childStyle->css_position_type_ == CSS_POSITION_ABSOLUTE) {
                    layoutAbsolute(renderer, child, width, height);
                    continue;
                }
                if(childStyle->css_position_type_ == CSS_POSITION_FIXED) {
                    layoutFixed(renderer, child);
                    continue;
                }
                currentColumnWithoutAbsoluteCount++;
                totalUseHeightWithoutAbsolute += child->GetMeasuredSize().height_
                        + child->GetStyle()->margin_top_ + child->GetStyle()->margin_bottom_;
            }

            int adjustHeightStart = 0;
            int adjustHeightInterval = 0;
            if(currentColumnWithoutAbsoluteCount > 0) {
                if(itemStyle->flex_justify_content_ == CSSFLEX_JUSTIFY_FLEX_START) {
                    //no action
                }else if(itemStyle->flex_justify_content_ == CSSFLEX_JUSTIFY_FLEX_END) {
                    adjustHeightStart = availableHeight - totalUseHeightWithoutAbsolute;
                }else if(itemStyle->flex_justify_content_ == CSSFLEX_JUSTIFY_FLEX_CENTER) {
                    adjustHeightStart = (availableHeight - totalUseHeightWithoutAbsolute) / 2;
                }else if(totalUseHeightWithoutAbsolute <= availableWidth && itemStyle->flex_justify_content_ == CSSFLEX_JUSTIFY_SPACE_BETWEEN) {
                    if(currentColumnWithoutAbsoluteCount > 1) {
                        adjustHeightInterval = round(((float)(availableHeight - totalUseHeightWithoutAbsolute)) /
                                (currentColumnWithoutAbsoluteCount-1));
                    }
                }else if(totalUseHeightWithoutAbsolute <= availableWidth && itemStyle->flex_justify_content_ == CSSFLEX_JUSTIFY_SPACE_AROUND) {
                    float interval = ((float)(availableHeight - totalUseHeightWithoutAbsolute)) /
                            (currentColumnWithoutAbsoluteCount*2);
                    adjustHeightStart = round(interval);
                    adjustHeightInterval = round(interval*2);
                }
            }

            // 防止有absolute的孩子导致第一个布局的孩子判断不准确
            int firstShowIndex = 0;
            int childOriginX = itemStyle->padding_left_ + itemStyle->border_width_;
            int childOriginY = itemStyle->padding_top_  + itemStyle->border_width_ + adjustHeightStart;
            for (int i = 0, size = renderer->GetChildCount(); i < size; i++) {
                LayoutObject* child = (LayoutObject*)renderer->Find(i);
                CSSStyle* childStyle = child->GetStyle();

                CSSStyleType align = itemStyle->flex_align_items_;

                if(childStyle->visible_ == CSS_HIDDEN) {
                    child->Layout(0,0,0,0);
                    continue;
                }
                if(childStyle->css_position_type_ == CSS_POSITION_ABSOLUTE ||
                        childStyle->css_position_type_ == CSS_POSITION_FIXED ||
                        childStyle->css_display_type_ == CSS_DISPLAY_NONE) {
                    firstShowIndex++;
                    continue;
                }

                if(childStyle->flex_align_self_ != CSSFLEX_ALIGN_AUTO) {
                    align = childStyle->flex_align_self_;
                }
                int oldChildOriginX = childOriginX;

                childOriginX += childStyle->margin_left_;
                childOriginY += childStyle->margin_top_;
                if(i > firstShowIndex)
                    childOriginY += adjustHeightInterval;

                int adjustX = 0;
                int adjustWidth = CSS_UNDEFINED;
                if(align == CSSFLEX_ALIGN_FLEX_START) {
                    //do nothing
                }else if(align == CSSFLEX_ALIGN_FLEX_END) {
                    adjustX = availableWidth - childStyle->margin_right_
                            - child->GetMeasuredSize().width_;
                }else if(align == CSSFLEX_ALIGN_STRETCH) {
                    if (CSS_IS_UNDEFINED(childStyle->width_)) {
                        adjustWidth = availableWidth - childStyle->margin_right_ - childStyle->margin_left_;
                        adjustWidth = child->GetStyle()->ClampWidth(adjustWidth);
                    }
                }else if(align == CSSFLEX_ALIGN_CENTER) {
                    adjustX = (availableWidth  - childStyle->margin_right_ - childStyle->margin_left_
                            - child->GetMeasuredSize().width_)/2;
                }

                int l = adjustX + childOriginX;
                int t = childOriginY;
                int r = CSS_IS_UNDEFINED(adjustWidth) ?
                        childOriginX + child->GetMeasuredSize().width_ + adjustX : adjustWidth+childOriginX;
                int b = childOriginY + child->GetMeasuredSize().height_;

                child->Layout(l,t,r,b);

                childOriginX = oldChildOriginX;
                childOriginY += child->GetMeasuredSize().height_ + childStyle->margin_bottom_;
            }
        }else {
            layoutColumnWrap(renderer, width, height);
        }
    }

    void CSSStaticLayout::measureAbsolute(LayoutObject* renderer, int width, int height) {
        int w = CSS_IS_UNDEFINED(renderer->GetStyle()->width_)
                ? width : renderer->GetStyle()->width_;
        int h = CSS_IS_UNDEFINED(renderer->GetStyle()->height_)
                ? height : renderer->GetStyle()->height_;
        Size size = renderer->Measure(w, h);
        //renderer->SetMeasuredSize(size);
    }

    void CSSStaticLayout::measureFixed(LayoutObject* renderer) {
        int width = static_cast<RenderObject*>(renderer)->render_tree_host()->view_port().width_;
        int height = static_cast<RenderObject*>(renderer)->render_tree_host()->view_port().height_;
        renderer->Measure(width, height);
        //renderer->SetMeasuredSize(renderer->Measure(width, height));
    }

    void CSSStaticLayout::layoutAbsolute(LayoutObject* parent, LayoutObject* renderer,  int width, int height) {
        layoutFixedOrAbsolute(parent, renderer, width, height);
    }

    void CSSStaticLayout::layoutFixed(LayoutObject* parent, LayoutObject* renderer) {
        int width = static_cast<RenderObject*>(renderer)->render_tree_host()->view_port().width_;
        int height = static_cast<RenderObject*>(renderer)->render_tree_host()->view_port().height_;
        layoutFixedOrAbsolute(parent, renderer, width, height);
    }

    void CSSStaticLayout::layoutFixedOrAbsolute(LayoutObject* parent, LayoutObject* renderer,
                                              double width, double height) {
        CSSStyle* parentStyle = parent->GetStyle();
        CSSStyle* style = renderer->GetStyle();

        double l = 0;
        double r = 0;
        double t = 0;
        double b = 0;

        if (!CSS_IS_UNDEFINED(style->left_) && !CSS_IS_UNDEFINED(style->right_)) {
            if (CSS_IS_UNDEFINED(style->width_)) {
                l = style->left_ + style->margin_left_ + parentStyle->border_width_;
                r = width - style->right_ - style->margin_right_ + parentStyle->border_width_;
                if (l > r) {
                    r = l;
                }
            } else {
                l = style->left_ + style->margin_left_ + parentStyle->border_width_;
                r = l + renderer->GetMeasuredSize().width_;
            }
        } else if(!CSS_IS_UNDEFINED(style->left_)) {
            l = style->left_ + style->margin_left_ + parentStyle->border_width_;
            r = l + renderer->GetMeasuredSize().width_;
        } else if(!CSS_IS_UNDEFINED(style->right_)) {
            r = width - style->right_ - style->margin_right_ - parentStyle->border_width_;
            l = r - renderer->GetMeasuredSize().width_;
        } else {
            l = l + style->margin_left_ + parentStyle->border_width_;
            r = l + renderer->GetMeasuredSize().width_;
        }

        if (!CSS_IS_UNDEFINED(style->top_) && !CSS_IS_UNDEFINED(style->bottom_)) {
            if (CSS_IS_UNDEFINED(style->height_)) {
                t = style->top_ + style->margin_top_ + parentStyle->border_width_;
                b = height - style->bottom_ - style->margin_bottom_ + parentStyle->border_width_;
                if (t > b) {
                    b = t;
                }
            } else {
                t = style->top_ + style->margin_top_ + parentStyle->border_width_;
                b = t + renderer->GetMeasuredSize().height_;
            }
        } else if(!CSS_IS_UNDEFINED(style->top_)) {
            t = style->top_ + style->margin_top_ + parentStyle->border_width_;
            b = t + renderer->GetMeasuredSize().height_;
        } else if(!CSS_IS_UNDEFINED(style->bottom_)) {
            b = height - style->bottom_ - style->margin_bottom_ - parentStyle->border_width_;
            t = b - renderer->GetMeasuredSize().height_;
        } else {
            t = t + style->margin_top_ + parentStyle->border_width_;
            b = t + renderer->GetMeasuredSize().height_;
        }

        renderer->Layout((int) l, (int) t, (int) r, (int) b);
    }
}
