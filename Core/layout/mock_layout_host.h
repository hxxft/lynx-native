// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_MOCK_LAYOUT_HOST_H_
#define LYNX_MOCK_LAYOUT_HOST_H_

#include "layout/layout_object.h"

#include <queue>

namespace lynx {
class MockLayoutHost {
 public:
  MockLayoutHost() { 
    SetUp();
  }
  ~MockLayoutHost() {
    TearDown(); 
  }

  CSSStyleConfig* config() { return &config_; }
  LayoutObject* body() { return root_; } 

 private:
  void SetUp() {
    CSSStyle::Initialize(&config_); 
    CSSStyle style(config(), 1, 750, 750);
    style.SetValue("flexDirection", "column");
    root_ = new LayoutObject;
    root_->set_css_style(style);
  }

  void TearDown() {
    std::queue<Node*> node_queue;
    node_queue.push(root_);
    while (!node_queue.empty()) {
      Node* node = node_queue.front();
      Node* child = static_cast<LayoutObject*>(node)->FirstChild();
      while (child) {
        node_queue.push(child);
        child = child->Next();
      }
      delete node;
      node_queue.pop(); 
    }
    root_ = nullptr;
  }

  CSSStyleConfig config_;
  LayoutObject* root_;
};
}  // namespace lynx

#endif
