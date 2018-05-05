//
// Created by chen on 19/04/2018.
//

#ifndef ANDROID_SPAN_H
#define ANDROID_SPAN_H

#include "render/render_object.h"

namespace lynx {
class Span : public RenderObject {
public:
  Span(jscore::ThreadManager *manager, const char *tag_name, uint64_t id,
       RenderTreeHost *host);
  virtual ~Span() {}
  virtual void FlushStyle() {};
  virtual void Layout(int left, int top, int right, int bottom) {}
  virtual void SetAttribute(const std::string& key, const std::string& value) override ;
  virtual const std::string &GetText() override;
  virtual void InsertChild(ContainerNode *child, int index) override;
};
} // namespace lynx

#endif // ANDROID_SPAN_H
