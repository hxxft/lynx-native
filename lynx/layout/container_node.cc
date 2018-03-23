// Copyright 2017 The Lynx Authors. All rights reserved.

#include "layout/container_node.h"

namespace lynx {

Node* ContainerNode::Find(int index) {
    if (index == 0) {
        return first_child_;
    }
    if (index < 0) {
        return nullptr;
    }

    Node* node = first_child_;

    while (index-- && node != nullptr) node = node->next_;

    return node;
}

int ContainerNode::Find(Node* node) {
    int index = 0;

    Node* find = first_child_;

    while (find != nullptr && find != node) {
        find = find->next_;
        ++index;
    }

    return index;
}

void ContainerNode::InsertChild(ContainerNode* child, Node* reference) {
    if (child_count_ == 0) {
        first_child_ = child;
        last_child_ = child;
    } else if (reference == nullptr) {
        last_child_->next_ = child;
        child->previous_ = last_child_;
        last_child_ = child;
    } else {
        Node* pre = reference->previous_;
        if (pre != NULL) {
            pre->next_ = child;
            child->previous_ = pre;
            child->next_ = reference;
            reference->previous_ = child;
        } else {
            child->next_ = first_child_;
            first_child_->previous_ = child;
            first_child_ = child;
        }
    }
    child->parent_ = this;
    ++child_count_;
}

void ContainerNode::InsertChild(ContainerNode* child) {
    InsertChild(child, -1);
}

void ContainerNode::InsertChild(ContainerNode *child, int index) {
    if (child->parent_) {
        child->parent_->RemoveChild(child);
    }
    InsertChild(child, Find(index));
}

void ContainerNode::RemoveChild(ContainerNode* child) {
    if (child == nullptr || child_count_ == 0)
        return;
    Node* pre = child->previous_;
    Node* next = child->next_;

    child->parent_ = nullptr;
    if (pre == nullptr && next == nullptr) {
        first_child_ = nullptr;
        last_child_ = nullptr;
    } else if (pre == nullptr) {
        next->previous_ = pre;
        first_child_ = next;
    } else if (next == nullptr) {
        pre->next_ = next;
        last_child_ = pre;
    } else {
        next->previous_ = pre;
        pre->next_ = next;
    }
    child_count_--;
}

Node* ContainerNode::RemoveChild(int index) {
    Node* node = Find(index);
    RemoveChild(static_cast<ContainerNode*>(node));
    return node;
}
}  // namespace lynx
