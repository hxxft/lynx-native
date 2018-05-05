//
// Created by chen on 19/04/2018.
//

#include "render/span.h"
#include "render/impl/render_object_impl.h"

namespace lynx {
Span::Span(jscore::ThreadManager *manager, const char *tag_name, uint64_t id,
           RenderTreeHost *host)
    : RenderObject(tag_name, LYNX_SPAN, id, NULL, host) {}

const std::string &Span::GetText() {
    text_ = "";
    RenderObject* child = static_cast<RenderObject*>(FirstChild());
    while (child) {
        text_.append(child->GetText());
        child = static_cast<RenderObject*>(child->Next());
    }

  return text_;
}

        void Span::SetAttribute(const std::string& key, const std::string& value){
            attributes_[key] = value;
        }

        void Span::InsertChild(ContainerNode *child, int index) {
            LayoutObject::InsertChild(child,index);
        }

} // namespace lynx