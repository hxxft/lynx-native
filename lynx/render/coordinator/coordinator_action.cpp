// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/coordinator/coordinator_action.h"

namespace lynx {

    double CoordinatorAction::translate_x_ = 0;
    double CoordinatorAction::translate_y_ = 0;
    double CoordinatorAction::scale_x_ = 0;
    double CoordinatorAction::scale_y_ = 0;
    double CoordinatorAction::rotate_x_ = 0;
    double CoordinatorAction::rotate_y_ = 0;
    double CoordinatorAction::pivot_x_ = 0;
    double CoordinatorAction::pivot_y_ = 0;
    double CoordinatorAction::alpha_ = 0;
    double CoordinatorAction::offset_top_ = 0;
    double CoordinatorAction::offset_bottom_ = 0;
    double CoordinatorAction::offset_left_ = 0;
    double CoordinatorAction::offset_right_ = 0;
    bool CoordinatorAction::consumed_ = false;
    double CoordinatorAction::duration_ = 0;
    std::string CoordinatorAction::event_ = "";
    lepus::Value CoordinatorAction::params_for_event_ = NULL;

    void CoordinatorAction::Reset() {
        translate_x_ = kNotSet;
        translate_y_ = kNotSet;
        scale_x_ = kNotSet;
        scale_y_ = kNotSet;
        rotate_x_ = kNotSet;
        rotate_y_ = kNotSet;
        pivot_x_ = kNotSet;
        pivot_y_ = kNotSet;
        alpha_ = kNotSet;
        offset_top_ = kNotSet;
        offset_bottom_ = kNotSet;
        offset_left_ = kNotSet;
        offset_right_ = kNotSet;
        consumed_ = false;
        duration_ = kNotSet;
        event_ = "";
        params_for_event_ = NULL;
    }

}