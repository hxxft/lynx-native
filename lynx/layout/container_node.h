// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_LAYOUT_CONTAINER_NODE_H_
#define LYNX_LAYOUT_CONTAINER_NODE_H_

#include "layout/node.h"

namespace lynx {
class ContainerNode : public Node {
 public:
    ContainerNode() :
        parent_(NULL),
        first_child_(NULL),
        last_child_(NULL),
        child_count_(0) {
        }
    virtual ~ContainerNode() {}
    virtual void InsertChild(ContainerNode* child, int index);
    virtual void RemoveChild(ContainerNode* child);

    void InsertChild(ContainerNode* child);
    void InsertChild(ContainerNode* child, Node* reference);
    Node* RemoveChild(int index);
    Node* FirstChild() { return first_child_; }
    Node* LastChild() { return last_child_; }
    Node* Find(int index);
    int Find(Node* node);

    int GetChildCount() {return child_count_;}
    
    ContainerNode* parent() {
        return parent_;
    }

 protected:
    ContainerNode* parent_;

 private:
    Node* first_child_;
    Node* last_child_;

    int child_count_;
};
}  // namespace lynx

#endif  // LYNX_LAYOUT_CONTAINER_NODE_H_
