// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_LABEL_H_
#define LYNX_RENDER_LABEL_H_

#include "render/render_object.h"

namespace lynx {
class TextNode : public RenderObject {
public:
    TextNode(jscore::ThreadManager* manager,
             const char* tag_name,
             uint64_t id,
             RenderTreeHost* host);
    virtual ~TextNode();

    virtual void SetText(const std::string& text);
    friend class Label;
private:
    RenderObject* label_;
};

class Label : public RenderObject {
public:
    Label(jscore::ThreadManager* manager,
        const char* tag_name,
        uint64_t id,
        RenderTreeHost* host);
    virtual ~Label() {}
    virtual base::Size Measure(int width, int height);
    virtual void Layout(int left, int top, int right, int bottom);
    virtual void InsertChild(ContainerNode* child, int index);
    virtual void InsertBefore(RenderObject* child, RenderObject* reference);
    TextNode* text_node() {
        return text_node_;
    };
private:
    TextNode* text_node_;
};
}  // namespace lynx

#endif  // LYNX_RENDER_LABEL_H_
