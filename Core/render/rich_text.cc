//
// Created by chen on 19/04/2018.
//

#include "render/rich_text.h"
#include "label_measurer.h"
#include "render/impl/render_object_impl.h"
#include "render/render_object.h"
#include <vector>

namespace lynx {


RichText::RichText(jscore::ThreadManager *manager, const char *tag_name,
                   uint64_t id, RenderTreeHost *host)
    : Label(manager, tag_name, id, host) {}

base::Size RichText::MeasureTextSize(const base::Size &size) {

  std::vector<std::string> texts;
  std::vector<CSSStyle> styles;

  if (GetChildCount() != 0) {
    RenderObject* child = static_cast<RenderObject*>(FirstChild());
    while (child) {
      auto t = child->GetText();
      auto s = child->css_style();
      texts.push_back(t);
      styles.push_back(s);
      child = static_cast<RenderObject*>(child->Next());
    }
    return LabelMeasurer::MeasureSpanSizeAndSetTextLayout(this, size, texts,
                                                          styles);
  } else {
    return Label::MeasureTextSize(size);
  }
}

void RichText::InsertChild(ContainerNode *child, int index) {
    LayoutObject::InsertChild(child,index);
}

} // namespace lynx