//
// Created by chen on 19/04/2018.
//

#ifndef ANDROID_RICH_TEXT_H
#define ANDROID_RICH_TEXT_H

#include "render/label.h"
#include <map>
#include "render/render_object.h"

namespace lynx {
class RichText : public Label {
public:
  RichText(jscore::ThreadManager *manager, const char *tag_name, uint64_t id,
           RenderTreeHost *host);
  virtual ~RichText() {}

  virtual base::Size MeasureTextSize(const base::Size &size) override;
  virtual void InsertChild(ContainerNode *child, int index) override;

};
} // namespace lynx

#endif // ANDROID_RICH_TEXT_H
