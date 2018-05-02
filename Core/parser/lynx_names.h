// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_PARSER_LYNX_NAMES_H_
#define LYNX_PARSER_LYNX_NAMES_H_

#include <string>
#include <set>

namespace parser {
    static const std::string kBodyTag = "body";
    static const std::string kDivTag = "div";
    static const std::string kImgTag = "img";
    static const std::string kScriptTag = "script";

    static const std::string kStyleAttr = "style";
    static const std::string kSrcAttr = "src";
    static const std::string kIDAttr = "id";
    static const std::string kTagAttr = "tag";

    static const std::set<std::string> kSelfClosingTags = {
       "area", "base", "br", "col", "command", "embed",
       "hr", "img", "input", "keygen", "link", "meta",
       "param", "source", "track", "wbr"
    };
}  // namespace parser

#endif  // LYNX_PARSER_LYNX_NAMES_H_
