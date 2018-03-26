// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/coordinator/coordinator_action.h"

namespace lynx {

    double CoordinatorAction::translate_x_ = 0;
    double CoordinatorAction::translate_y_ = 0;
    double CoordinatorAction::scale_x_ = 0;
    double CoordinatorAction::scale_y_ = 0;
    double CoordinatorAction::rotate_x_ = 0;
    double CoordinatorAction::rotate_y_ = 0;
    double CoordinatorAction::origin_x_ = 0;
    double CoordinatorAction::origin_y_ = 0;
    double CoordinatorAction::opacity_ = 0;
    double CoordinatorAction::top_offset_ = 0;
    double CoordinatorAction::bottom_offset_ = 0;
    double CoordinatorAction::left_offset_ = 0;
    double CoordinatorAction::right_offset_ = 0;
    bool CoordinatorAction::consumed_ = false;
    double CoordinatorAction::duration_ = 0;
    double CoordinatorAction::timing_function_ = 0;
    std::string CoordinatorAction::event_ = "";
    lepus::Value CoordinatorAction::params_for_event_ = NULL;

    void CoordinatorAction::Reset() {
        translate_x_ = kNotSet;
        translate_y_ = kNotSet;
        scale_x_ = kNotSet;
        scale_y_ = kNotSet;
        rotate_x_ = kNotSet;
        rotate_y_ = kNotSet;
        origin_x_ = kNotSet;
        origin_y_ = kNotSet;
        opacity_ = kNotSet;
        top_offset_ = kNotSet;
        bottom_offset_ = kNotSet;
        left_offset_ = kNotSet;
        right_offset_ = kNotSet;
        consumed_ = false;
        duration_ = kNotSet;
        event_ = "";
        params_for_event_ = NULL;
        timing_function_ = kNotSet;
    }

}