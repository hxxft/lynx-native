// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_CONFIG_GLOBAL_CONFIG_DATA_H_
#define LYNX_CONFIG_GLOBAL_CONFIG_DATA_H_

#include "loader/cache/source_cache_manager.h"
#include "layout/css_style_config.h"

namespace config {
class GlobalConfigData {
 public:
    static GlobalConfigData* GetInstance();

    GlobalConfigData()
        : is_initialized_(false),
          screen_density_(0),
          screen_width_(0),
          screen_height_(0),
          style_config_(),
          cache_manager_() {
    }


    ~GlobalConfigData() {
    }

    inline double screen_density() {
        return screen_density_;
    }

    inline int screen_width() {
        return screen_width_;
    }

    inline int screen_height() {
        return screen_height_;
    }

    void SetScreenConfig(int width, int height, double density) {
        screen_width_ = width / density;
        screen_height_ = height / density;
        screen_density_ = density;
    }

    bool is_initialized() {
        return is_initialized_;
    }

    lynx::CSSStyleConfig* style_config() {
        return &style_config_;
    }

    loader::SourceCacheManager& cache_manager() {
        return cache_manager_;
    }

 private:
    bool is_initialized_;
    double screen_density_;
    int screen_width_;
    int screen_height_;

    lynx::CSSStyleConfig style_config_;

    loader::SourceCacheManager cache_manager_;
};

}  // namespace config

#endif  // LYNX_CONFIG_GLOBAL_CONFIG_DATA_H_
