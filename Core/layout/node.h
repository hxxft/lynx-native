// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_LAYOUT_NODE_H_
#define LYNX_LAYOUT_NODE_H_

#include <stddef.h>

#include "base/ref_counted_ptr.h"

namespace lynx {
class Node {
 public:
    Node() : previous_(NULL), next_(NULL) {}
    virtual ~Node() {}
    inline Node* Next() { return next_; }
    inline Node* Previous() { return previous_; }
    friend class ContainerNode;
 private:
    Node* previous_;
    Node* next_;
};
}  // namespace lynx

#endif  // LYNX_LAYOUT_NODE_H_
