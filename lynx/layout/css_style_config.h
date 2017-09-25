// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_LAYOUT_CSS_STYLE_CONFIG_H_
#define LYNX_LAYOUT_CSS_STYLE_CONFIG_H_

#include <map>
#include <string>

namespace lynx {
class CSSStyle;
class CSSStyleConfig {
 public:
    typedef void (CSSStyle::*StyleFunc)(const std::string&);
    typedef std::map<std::string, StyleFunc> StyleFuncMap;

    StyleFuncMap* func_map() {
        return &func_map_;
    }

 private:
    StyleFuncMap func_map_;
};
}  // namespace lynx

#endif  // LYNX_LAYOUT_CSS_STYLE_CONFIG_H_
