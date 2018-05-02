
#include "inspector/inspector_css_agent.h"

#include "render/render_object.h"

#include "runtime/runtime.h"
#include "render/render_tree_host.h"
#include "render/label.h"
#include "layout/css_style.h"
#include "layout/css_type.h"

#include <sstream>

namespace {
    lynx::RenderObject* findRenderer(lynx::RenderObject* root, uint64_t id) {
        if(root == nullptr) return nullptr;
        if(root->id() == id) return root;
        
        int child_count = root->GetChildCount();
        if(root->GetChildCount() > 0) {
            for(int i = 0; i < child_count; ++i) {
                lynx::RenderObject* renderer = findRenderer(const_cast<lynx::RenderObject*>(root->Get(i)), id);
                if(renderer) return renderer;
            }
        }
        return nullptr;
    }
    
    Json::Value getInlineStyle(lynx::RenderObject* root, uint64_t id) {
        int count = 0;
        lynx::RenderObject* renderer = findRenderer(root, id);
        Json::Value propertys(Json::arrayValue);
        if(renderer && !renderer->styles().empty()) {
            for(auto iter = renderer->styles().begin();
                iter != renderer->styles().end(); ++iter) {
                Json::Value property;
                property["name"] = iter->first;
                property["value"] = iter->second;
                propertys[count++] = property;
            }
        }
        return propertys;
    }
    
    Json::Value makeStyleProperty(const std::string& key, const std::string& value) {
        Json::Value property;
        property["name"] = key;
        property["value"] = value;
        return property;
    }
    
    Json::Value makeStylePropertyIm(const std::string& key, int value) {
        Json::Value property;
        property["name"] = key;
        property["value"] = value;
        return property;
    }
    
    Json::Value makeStylePropertyIm(const std::string& key, double value) {
        Json::Value property;
        property["name"] = key;
        property["value"] = value;
        return property;
    }
    
    Json::Value makeStyleProperty(const std::string& key, int value) {
        Json::Value property;
        std::stringstream ss;
        ss << value <<"px";
        property["name"] = key;
        property["value"] = ss.str();
        return property;
    }
    
    Json::Value makeStyleProperty(const std::string& key, double value) {
        Json::Value property;
        property["name"] = key;
        if(value == CSS_UNDEFINED) {
            property["value"] = "none";
        }else{
            std::stringstream ss;
            ss << value <<"px";
            property["value"] = ss.str();
        }        
        return property;
    }
    
    Json::Value makeStyleProperty(const std::string& key, lynx::CSSColor color) {
        Json::Value property;
        std::stringstream ss;
        ss<<"rgba("<<(int)color.r_<<", "<<(int)color.g_<<", "<<(int)color.b_<<", "<<color.a_<<")";
        property["name"] = key;
        property["value"] = ss.str();
        return property;
    }
    
    Json::Value makeDefaultComputedStyle(lynx::RenderObject* root, uint64_t id) {
        int count = 0;
        Json::Value propertys(Json::arrayValue);
//        int len = arraysize(kDefaultStytles);
//        for(int i = 0; i < len; i = i + 2) {
//            int index = (i+1)>>2;
//            propertys[index] = makeStyleProperty(kDefaultStytles[i], kDefaultStytles[i+1]);
//        }
        return propertys;
    }
    
    Json::Value getComputedStyle(lynx::RenderObject* root, uint64_t id) {
        int count = 0;
        lynx::RenderObject* renderer = findRenderer(root, id);
        const lynx::CSSStyle* style = &(renderer->css_style());
        Json::Value propertys(Json::arrayValue);
        propertys[count++] = makeStyleProperty("height", renderer->measured_size().height_);
        propertys[count++] = makeStyleProperty("width", renderer->measured_size().width_);
        propertys[count++] = makeStyleProperty("left", "auto");
        propertys[count++] = makeStyleProperty("right", "auto");
        propertys[count++] = makeStyleProperty("top", "auto");
        propertys[count++] = makeStyleProperty("bottom", "auto");
        propertys[count++] = makeStyleProperty("min-width", style->min_width_);
        propertys[count++] = makeStyleProperty("max-width", style->max_width_);
        propertys[count++] = makeStyleProperty("min-height", style->min_height_);
        propertys[count++] = makeStyleProperty("max-height", style->max_height_);
        propertys[count++] = makeStyleProperty("margin-left", style->margin_left_);
        propertys[count++] = makeStyleProperty("margin-right", style->margin_right_);
        propertys[count++] = makeStyleProperty("margin-top", style->margin_top_);
        propertys[count++] = makeStyleProperty("margin-bottom", style->margin_bottom_);
        propertys[count++] = makeStyleProperty("padding-left", style->padding_left_);
        propertys[count++] = makeStyleProperty("padding-right", style->padding_right_);
        propertys[count++] = makeStyleProperty("padding-top", style->padding_top_);
        propertys[count++] = makeStyleProperty("padding-bottom", style->padding_bottom_);
        propertys[count++] = makeStyleProperty("visible", lynx::MapCSSType(style->visible_));
        propertys[count++] = makeStyleProperty("background-color", style->background_color_);
        propertys[count++] = makeStyleProperty("border-left-width", style->border_width_);
        propertys[count++] = makeStyleProperty("border-right-width", style->border_width_);
        propertys[count++] = makeStyleProperty("border-top-width", style->border_width_);
        propertys[count++] = makeStyleProperty("border-bottom-width", style->border_width_);
        propertys[count++] = makeStyleProperty("border-color", style->border_color_);
        propertys[count++] = makeStyleProperty("border-radius", style->border_radius_);
//        propertys[count++] = makeStylePropertyIm("opacity", style->opacity_/255.0);
//        //propertys[count++] = makeStylePropertyIm("flex-grow", style->flex_);
        propertys[count++] = makeStyleProperty("flex-direction", lynx::MapCSSType(style->flex_direction_));
        propertys[count++] = makeStyleProperty("flex-wrap", lynx::MapCSSType(style->flex_wrap_));
        propertys[count++] = makeStyleProperty("justify-content", lynx::MapCSSType(style->flex_justify_content_));
        propertys[count++] = makeStyleProperty("align-items", lynx::MapCSSType(style->flex_align_items_));
        propertys[count++] = makeStyleProperty("align-self", lynx::MapCSSType(style->flex_align_self_));
        propertys[count++] = makeStyleProperty("position", lynx::MapCSSType(style->css_position_type_));
        propertys[count++] = makeStyleProperty("display", lynx::MapCSSType(style->css_display_type_));
//        if(renderer->render_object_type() == lynx::LYNX_LABEL) {
//            propertys[count++] = makeStyleProperty("font-weight", lynx::MapTextStyle(style->font_weight_));
//            propertys[count++] = makeStyleProperty("font-size", style->font_size_);
//            propertys[count++] = makeStyleProperty("color", style->font_color_);
//            propertys[count++] = makeStyleProperty("text-overflow", lynx::MapTextStyle(style->text_overflow_));
//            propertys[count++] = makeStyleProperty("white-space", lynx::MapTextStyle(style->text_white_space_));
//            propertys[count++] = makeStyleProperty("text-align", lynx::MapTextStyle(style->text_align_));
//            propertys[count++] = makeStyleProperty("text-decoration", lynx::MapTextStyle(style->text_decoration_));
//            propertys[count++] = makeStyleProperty("line-height", style->line_height_);
//        }
//
//        if(renderer->render_object_type() == lynx::LYNX_IMAGEVIEW) {
//            propertys[count++] = makeStyleProperty("object-fit", lynx::MapImageStyle(style->css_object_fit_));
//        }
//        propertys[count++] = makeStylePropertyIm("z-index", style->zindex_);
//
        return propertys;
    }
}

namespace debug {
    
    InspectorCSSAgent::InspectorCSSAgent(jscore::Runtime* runtime):runtime_(runtime) {
        functions_map_["CSS.getMatchedStylesForNode"] = &InspectorCSSAgent::GetMatchedStylesForNode;
        functions_map_["CSS.getComputedStyleForNode"] = &InspectorCSSAgent::GetComputedStyleForNode;
        functions_map_["CSS.getInlineStylesForNode"] = &InspectorCSSAgent::GetInlineStylesForNode;
    }
    
    InspectorCSSAgent::~InspectorCSSAgent(){
        
    }
    
    Json::Value InspectorCSSAgent::GetMatchedStylesForNode(const Json::Value& params) {
        #ifndef INSPECTOR_TEST
        Json::Value root;
        Json::Value css_style;
        Json::Value computed_style = getInlineStyle(runtime_->render_tree_host()->render_root(), params["nodeId"].asInt64());
        css_style["cssProperties"] = computed_style;
        css_style["shorthandEntries"] = Json::Value(Json::arrayValue);
        root["inlineStyle"] = css_style;
        return root;
#else
        std::string style = "{\"id\":29,\"result\":{\"inlineStyle\":{\"styleSheetId\":\"38569.6\",\"cssProperties\":[],\"shorthandEntries\":[],\"cssText\":\"\",\"range\":{\"startLine\":0,\"startColumn\":0,\"endLine\":0,\"endColumn\":0}},\"matchedCSSRules\":[{\"rule\":{\"selectorList\":{\"selectors\":[{\"text\":\"body\"}],\"text\":\"body\"},\"origin\":\"user-agent\",\"style\":{\"cssProperties\":[{\"name\":\"display\",\"value\":\"block\"},{\"name\":\"margin-top\",\"value\":\"8px\"},{\"name\":\"margin-right\",\"value\":\"8px\"},{\"name\":\"margin-bottom\",\"value\":\"8px\"},{\"name\":\"margin-left\",\"value\":\"8px\"}],\"shorthandEntries\":[{\"name\":\"margin\",\"value\":\"8px\"}]},\"media\":[]},\"matchingSelectors\":[0]}],\"pseudoElements\":[],\"inherited\":[{\"matchedCSSRules\":[{\"rule\":{\"selectorList\":{\"selectors\":[{\"text\":\"html\"}],\"text\":\"html\"},\"origin\":\"user-agent\",\"style\":{\"cssProperties\":[{\"name\":\"display\",\"value\":\"block\"}],\"shorthandEntries\":[]},\"media\":[]},\"matchingSelectors\":[0]}]}],\"cssKeyframesRules\":[]}}";
        Json::Value content;
        Json::Reader reader;
        reader.parse(style, content, false);
        return content["result"];
#endif
    }
    
    Json::Value InspectorCSSAgent::GetComputedStyleForNode(const Json::Value& params) {
        #ifndef INSPECTOR_TEST
        Json::Value root;
        Json::Value computed_style = getComputedStyle(runtime_->render_tree_host()->render_root(), params["nodeId"].asInt64());
        root["computedStyle"] = computed_style;
        return root;
#else
        std::string style = "{\"id\":30,\"result\":{\"computedStyle\":[{\"name\":\"animation-delay\",\"value\":\"0s\"},{\"name\":\"animation-direction\",\"value\":\"normal\"},{\"name\":\"animation-duration\",\"value\":\"0s\"},{\"name\":\"animation-fill-mode\",\"value\":\"none\"},{\"name\":\"animation-iteration-count\",\"value\":\"1\"},{\"name\":\"animation-name\",\"value\":\"none\"},{\"name\":\"animation-play-state\",\"value\":\"running\"},{\"name\":\"animation-timing-function\",\"value\":\"ease\"},{\"name\":\"transition-delay\",\"value\":\"0s\"},{\"name\":\"transition-duration\",\"value\":\"0s\"},{\"name\":\"transition-property\",\"value\":\"all\"},{\"name\":\"transition-timing-function\",\"value\":\"ease\"},{\"name\":\"color\",\"value\":\"rgb(0, 0, 0)\"},{\"name\":\"direction\",\"value\":\"ltr\"},{\"name\":\"font-family\",\"value\":\"Times\"},{\"name\":\"font-feature-settings\",\"value\":\"normal\"},{\"name\":\"font-kerning\",\"value\":\"auto\"},{\"name\":\"font-size\",\"value\":\"16px\"},{\"name\":\"font-size-adjust\",\"value\":\"none\"},{\"name\":\"font-stretch\",\"value\":\"100%\"},{\"name\":\"font-style\",\"value\":\"normal\"},{\"name\":\"font-variant-caps\",\"value\":\"normal\"},{\"name\":\"font-variant-east-asian\",\"value\":\"normal\"},{\"name\":\"font-variant-ligatures\",\"value\":\"normal\"},{\"name\":\"font-variant-numeric\",\"value\":\"normal\"},{\"name\":\"font-variation-settings\",\"value\":\"normal\"},{\"name\":\"font-weight\",\"value\":\"400\"},{\"name\":\"text-orientation\",\"value\":\"mixed\"},{\"name\":\"text-rendering\",\"value\":\"auto\"},{\"name\":\"-webkit-font-smoothing\",\"value\":\"auto\"},{\"name\":\"-webkit-locale\",\"value\":\"auto\"},{\"name\":\"-webkit-text-orientation\",\"value\":\"vertical-right\"},{\"name\":\"-webkit-writing-mode\",\"value\":\"horizontal-tb\"},{\"name\":\"writing-mode\",\"value\":\"horizontal-tb\"},{\"name\":\"zoom\",\"value\":\"1\"},{\"name\":\"align-content\",\"value\":\"normal\"},{\"name\":\"align-items\",\"value\":\"normal\"},{\"name\":\"align-self\",\"value\":\"auto\"},{\"name\":\"alignment-baseline\",\"value\":\"auto\"},{\"name\":\"all\",\"value\":\"\"},{\"name\":\"backdrop-filter\",\"value\":\"none\"},{\"name\":\"backface-visibility\",\"value\":\"visible\"},{\"name\":\"background-attachment\",\"value\":\"scroll\"},{\"name\":\"background-blend-mode\",\"value\":\"normal\"},{\"name\":\"background-clip\",\"value\":\"border-box\"},{\"name\":\"background-color\",\"value\":\"rgba(0, 0, 0, 0)\"},{\"name\":\"background-image\",\"value\":\"none\"},{\"name\":\"background-origin\",\"value\":\"padding-box\"},{\"name\":\"background-position-x\",\"value\":\"0%\"},{\"name\":\"background-position-y\",\"value\":\"0%\"},{\"name\":\"background-repeat-x\",\"value\":\"\"},{\"name\":\"background-repeat-y\",\"value\":\"\"},{\"name\":\"background-size\",\"value\":\"auto\"},{\"name\":\"baseline-shift\",\"value\":\"0px\"},{\"name\":\"block-size\",\"value\":\"913px\"},{\"name\":\"border-bottom-color\",\"value\":\"rgb(0, 0, 0)\"},{\"name\":\"border-bottom-left-radius\",\"value\":\"0px\"},{\"name\":\"border-bottom-right-radius\",\"value\":\"0px\"},{\"name\":\"border-bottom-style\",\"value\":\"none\"},{\"name\":\"border-bottom-width\",\"value\":\"0px\"},{\"name\":\"border-collapse\",\"value\":\"separate\"},{\"name\":\"border-image-outset\",\"value\":\"0px\"},{\"name\":\"border-image-repeat\",\"value\":\"stretch\"},{\"name\":\"border-image-slice\",\"value\":\"100%\"},{\"name\":\"border-image-source\",\"value\":\"none\"},{\"name\":\"border-image-width\",\"value\":\"1\"},{\"name\":\"border-left-color\",\"value\":\"rgb(0, 0, 0)\"},{\"name\":\"border-left-style\",\"value\":\"none\"},{\"name\":\"border-left-width\",\"value\":\"0px\"},{\"name\":\"border-right-color\",\"value\":\"rgb(0, 0, 0)\"},{\"name\":\"border-right-style\",\"value\":\"none\"},{\"name\":\"border-right-width\",\"value\":\"0px\"},{\"name\":\"border-top-color\",\"value\":\"rgb(0, 0, 0)\"},{\"name\":\"border-top-left-radius\",\"value\":\"0px\"},{\"name\":\"border-top-right-radius\",\"value\":\"0px\"},{\"name\":\"border-top-style\",\"value\":\"none\"},{\"name\":\"border-top-width\",\"value\":\"0px\"},{\"name\":\"bottom\",\"value\":\"auto\"},{\"name\":\"box-shadow\",\"value\":\"none\"},{\"name\":\"box-sizing\",\"value\":\"content-box\"},{\"name\":\"break-after\",\"value\":\"auto\"},{\"name\":\"break-before\",\"value\":\"auto\"},{\"name\":\"break-inside\",\"value\":\"auto\"},{\"name\":\"buffered-rendering\",\"value\":\"auto\"},{\"name\":\"caption-side\",\"value\":\"top\"},{\"name\":\"caret-color\",\"value\":\"rgb(0, 0, 0)\"},{\"name\":\"clear\",\"value\":\"none\"},{\"name\":\"clip\",\"value\":\"auto\"},{\"name\":\"clip-path\",\"value\":\"none\"},{\"name\":\"clip-rule\",\"value\":\"nonzero\"},{\"name\":\"color-interpolation\",\"value\":\"sRGB\"},{\"name\":\"color-interpolation-filters\",\"value\":\"linearRGB\"},{\"name\":\"color-rendering\",\"value\":\"auto\"},{\"name\":\"column-count\",\"value\":\"auto\"},{\"name\":\"column-fill\",\"value\":\"balance\"},{\"name\":\"column-gap\",\"value\":\"normal\"},{\"name\":\"column-rule-color\",\"value\":\"rgb(0, 0, 0)\"},{\"name\":\"column-rule-style\",\"value\":\"none\"},{\"name\":\"column-rule-width\",\"value\":\"0px\"},{\"name\":\"column-span\",\"value\":\"none\"},{\"name\":\"column-width\",\"value\":\"auto\"},{\"name\":\"contain\",\"value\":\"none\"},{\"name\":\"content\",\"value\":\"\"},{\"name\":\"counter-increment\",\"value\":\"none\"},{\"name\":\"counter-reset\",\"value\":\"none\"},{\"name\":\"cursor\",\"value\":\"auto\"},{\"name\":\"cx\",\"value\":\"0px\"},{\"name\":\"cy\",\"value\":\"0px\"},{\"name\":\"d\",\"value\":\"none\"},{\"name\":\"display\",\"value\":\"block\"},{\"name\":\"dominant-baseline\",\"value\":\"auto\"},{\"name\":\"empty-cells\",\"value\":\"show\"},{\"name\":\"fill\",\"value\":\"rgb(0, 0, 0)\"},{\"name\":\"fill-opacity\",\"value\":\"1\"},{\"name\":\"fill-rule\",\"value\":\"nonzero\"},{\"name\":\"filter\",\"value\":\"none\"},{\"name\":\"flex-basis\",\"value\":\"auto\"},{\"name\":\"flex-direction\",\"value\":\"row\"},{\"name\":\"flex-grow\",\"value\":\"0\"},{\"name\":\"flex-shrink\",\"value\":\"1\"},{\"name\":\"flex-wrap\",\"value\":\"nowrap\"},{\"name\":\"float\",\"value\":\"none\"},{\"name\":\"flood-color\",\"value\":\"rgb(0, 0, 0)\"},{\"name\":\"flood-opacity\",\"value\":\"1\"},{\"name\":\"font-display\",\"value\":\"\"},{\"name\":\"grid-auto-columns\",\"value\":\"auto\"},{\"name\":\"grid-auto-flow\",\"value\":\"row\"},{\"name\":\"grid-auto-rows\",\"value\":\"auto\"},{\"name\":\"grid-column-end\",\"value\":\"auto\"},{\"name\":\"grid-column-gap\",\"value\":\"0px\"},{\"name\":\"grid-column-start\",\"value\":\"auto\"},{\"name\":\"grid-row-end\",\"value\":\"auto\"},{\"name\":\"grid-row-gap\",\"value\":\"0px\"},{\"name\":\"grid-row-start\",\"value\":\"auto\"},{\"name\":\"grid-template-areas\",\"value\":\"none\"},{\"name\":\"grid-template-columns\",\"value\":\"none\"},{\"name\":\"grid-template-rows\",\"value\":\"none\"},{\"name\":\"height\",\"value\":\"913px\"},{\"name\":\"hyphens\",\"value\":\"manual\"},{\"name\":\"image-orientation\",\"value\":\"0deg\"},{\"name\":\"image-rendering\",\"value\":\"auto\"},{\"name\":\"inline-size\",\"value\":\"1567px\"},{\"name\":\"isolation\",\"value\":\"auto\"},{\"name\":\"justify-content\",\"value\":\"normal\"},{\"name\":\"justify-items\",\"value\":\"normal\"},{\"name\":\"justify-self\",\"value\":\"auto\"},{\"name\":\"left\",\"value\":\"auto\"},{\"name\":\"letter-spacing\",\"value\":\"normal\"},{\"name\":\"lighting-color\",\"value\":\"rgb(255, 255, 255)\"},{\"name\":\"line-break\",\"value\":\"auto\"},{\"name\":\"line-height\",\"value\":\"normal\"},{\"name\":\"line-height-step\",\"value\":\"0px\"},{\"name\":\"list-style-image\",\"value\":\"none\"},{\"name\":\"list-style-position\",\"value\":\"outside\"},{\"name\":\"list-style-type\",\"value\":\"disc\"},{\"name\":\"margin-bottom\",\"value\":\"8px\"},{\"name\":\"margin-left\",\"value\":\"8px\"},{\"name\":\"margin-right\",\"value\":\"8px\"},{\"name\":\"margin-top\",\"value\":\"8px\"},{\"name\":\"marker-end\",\"value\":\"none\"},{\"name\":\"marker-mid\",\"value\":\"none\"},{\"name\":\"marker-start\",\"value\":\"none\"},{\"name\":\"mask\",\"value\":\"none\"},{\"name\":\"mask-source-type\",\"value\":\"alpha\"},{\"name\":\"mask-type\",\"value\":\"luminance\"},{\"name\":\"max-block-size\",\"value\":\"none\"},{\"name\":\"max-height\",\"value\":\"none\"},{\"name\":\"max-inline-size\",\"value\":\"none\"},{\"name\":\"max-width\",\"value\":\"none\"},{\"name\":\"max-zoom\",\"value\":\"\"},{\"name\":\"min-block-size\",\"value\":\"0px\"},{\"name\":\"min-height\",\"value\":\"0px\"},{\"name\":\"min-inline-size\",\"value\":\"0px\"},{\"name\":\"min-width\",\"value\":\"0px\"},{\"name\":\"min-zoom\",\"value\":\"\"},{\"name\":\"mix-blend-mode\",\"value\":\"normal\"},{\"name\":\"object-fit\",\"value\":\"fill\"},{\"name\":\"object-position\",\"value\":\"50% 50%\"},{\"name\":\"offset-anchor\",\"value\":\"auto\"},{\"name\":\"offset-distance\",\"value\":\"0px\"},{\"name\":\"offset-path\",\"value\":\"none\"},{\"name\":\"offset-position\",\"value\":\"auto\"},{\"name\":\"offset-rotate\",\"value\":\"auto 0deg\"},{\"name\":\"opacity\",\"value\":\"1\"},{\"name\":\"order\",\"value\":\"0\"},{\"name\":\"orientation\",\"value\":\"\"},{\"name\":\"orphans\",\"value\":\"2\"},{\"name\":\"outline-color\",\"value\":\"rgb(0, 0, 0)\"},{\"name\":\"outline-offset\",\"value\":\"0px\"},{\"name\":\"outline-style\",\"value\":\"none\"},{\"name\":\"outline-width\",\"value\":\"0px\"},{\"name\":\"overflow-anchor\",\"value\":\"auto\"},{\"name\":\"overflow-wrap\",\"value\":\"normal\"},{\"name\":\"overflow-x\",\"value\":\"visible\"},{\"name\":\"overflow-y\",\"value\":\"visible\"},{\"name\":\"overscroll-behavior-x\",\"value\":\"auto\"},{\"name\":\"overscroll-behavior-y\",\"value\":\"auto\"},{\"name\":\"padding-bottom\",\"value\":\"0px\"},{\"name\":\"padding-left\",\"value\":\"0px\"},{\"name\":\"padding-right\",\"value\":\"0px\"},{\"name\":\"padding-top\",\"value\":\"0px\"},{\"name\":\"page\",\"value\":\"\"},{\"name\":\"paint-order\",\"value\":\"fill stroke markers\"},{\"name\":\"perspective\",\"value\":\"none\"},{\"name\":\"perspective-origin\",\"value\":\"783.5px 456.5px\"},{\"name\":\"pointer-events\",\"value\":\"auto\"},{\"name\":\"position\",\"value\":\"static\"},{\"name\":\"quotes\",\"value\":\"\"},{\"name\":\"r\",\"value\":\"0px\"},{\"name\":\"resize\",\"value\":\"none\"},{\"name\":\"right\",\"value\":\"auto\"},{\"name\":\"rotate\",\"value\":\"none\"},{\"name\":\"rx\",\"value\":\"auto\"},{\"name\":\"ry\",\"value\":\"auto\"},{\"name\":\"scale\",\"value\":\"none\"},{\"name\":\"scroll-behavior\",\"value\":\"auto\"},{\"name\":\"scroll-padding-block-end\",\"value\":\"auto\"},{\"name\":\"scroll-padding-block-start\",\"value\":\"auto\"},{\"name\":\"scroll-padding-bottom\",\"value\":\"auto\"},{\"name\":\"scroll-padding-inline-end\",\"value\":\"auto\"},{\"name\":\"scroll-padding-inline-start\",\"value\":\"auto\"},{\"name\":\"scroll-padding-left\",\"value\":\"auto\"},{\"name\":\"scroll-padding-right\",\"value\":\"auto\"},{\"name\":\"scroll-padding-top\",\"value\":\"auto\"},{\"name\":\"scroll-snap-align\",\"value\":\"none\"},{\"name\":\"scroll-snap-margin-block-end\",\"value\":\"auto\"},{\"name\":\"scroll-snap-margin-block-start\",\"value\":\"auto\"},{\"name\":\"scroll-snap-margin-bottom\",\"value\":\"auto\"},{\"name\":\"scroll-snap-margin-inline-end\",\"value\":\"auto\"},{\"name\":\"scroll-snap-margin-inline-start\",\"value\":\"auto\"},{\"name\":\"scroll-snap-margin-left\",\"value\":\"auto\"},{\"name\":\"scroll-snap-margin-right\",\"value\":\"auto\"},{\"name\":\"scroll-snap-margin-top\",\"value\":\"auto\"},{\"name\":\"scroll-snap-stop\",\"value\":\"normal\"},{\"name\":\"scroll-snap-type\",\"value\":\"none\"},{\"name\":\"shape-image-threshold\",\"value\":\"0\"},{\"name\":\"shape-margin\",\"value\":\"0px\"},{\"name\":\"shape-outside\",\"value\":\"none\"},{\"name\":\"shape-rendering\",\"value\":\"auto\"},{\"name\":\"size\",\"value\":\"\"},{\"name\":\"speak\",\"value\":\"normal\"},{\"name\":\"src\",\"value\":\"\"},{\"name\":\"stop-color\",\"value\":\"rgb(0, 0, 0)\"},{\"name\":\"stop-opacity\",\"value\":\"1\"},{\"name\":\"stroke\",\"value\":\"none\"},{\"name\":\"stroke-dasharray\",\"value\":\"none\"},{\"name\":\"stroke-dashoffset\",\"value\":\"0px\"},{\"name\":\"stroke-linecap\",\"value\":\"butt\"},{\"name\":\"stroke-linejoin\",\"value\":\"miter\"},{\"name\":\"stroke-miterlimit\",\"value\":\"4\"},{\"name\":\"stroke-opacity\",\"value\":\"1\"},{\"name\":\"stroke-width\",\"value\":\"1px\"},{\"name\":\"tab-size\",\"value\":\"8\"},{\"name\":\"table-layout\",\"value\":\"auto\"},{\"name\":\"text-align\",\"value\":\"start\"},{\"name\":\"text-align-last\",\"value\":\"auto\"},{\"name\":\"text-anchor\",\"value\":\"start\"},{\"name\":\"text-combine-upright\",\"value\":\"none\"},{\"name\":\"text-decoration-color\",\"value\":\"rgb(0, 0, 0)\"},{\"name\":\"text-decoration-line\",\"value\":\"none\"},{\"name\":\"text-decoration-skip\",\"value\":\"objects\"},{\"name\":\"text-decoration-style\",\"value\":\"solid\"},{\"name\":\"text-indent\",\"value\":\"0px\"},{\"name\":\"text-justify\",\"value\":\"auto\"},{\"name\":\"text-overflow\",\"value\":\"clip\"},{\"name\":\"text-shadow\",\"value\":\"none\"},{\"name\":\"text-size-adjust\",\"value\":\"auto\"},{\"name\":\"text-transform\",\"value\":\"none\"},{\"name\":\"text-underline-position\",\"value\":\"auto\"},{\"name\":\"top\",\"value\":\"auto\"},{\"name\":\"touch-action\",\"value\":\"auto\"},{\"name\":\"transform\",\"value\":\"none\"},{\"name\":\"transform-box\",\"value\":\"border-box\"},{\"name\":\"transform-origin\",\"value\":\"783.5px 456.5px\"},{\"name\":\"transform-style\",\"value\":\"flat\"},{\"name\":\"translate\",\"value\":\"none\"},{\"name\":\"unicode-bidi\",\"value\":\"normal\"},{\"name\":\"unicode-range\",\"value\":\"\"},{\"name\":\"user-select\",\"value\":\"auto\"},{\"name\":\"user-zoom\",\"value\":\"\"},{\"name\":\"vector-effect\",\"value\":\"none\"},{\"name\":\"vertical-align\",\"value\":\"baseline\"},{\"name\":\"visibility\",\"value\":\"visible\"},{\"name\":\"-webkit-app-region\",\"value\":\"no-drag\"},{\"name\":\"-webkit-appearance\",\"value\":\"none\"},{\"name\":\"-webkit-background-clip\",\"value\":\"border-box\"},{\"name\":\"-webkit-background-origin\",\"value\":\"padding-box\"},{\"name\":\"-webkit-border-after-color\",\"value\":\"rgb(0, 0, 0)\"},{\"name\":\"-webkit-border-after-style\",\"value\":\"none\"},{\"name\":\"-webkit-border-after-width\",\"value\":\"0px\"},{\"name\":\"-webkit-border-before-color\",\"value\":\"rgb(0, 0, 0)\"},{\"name\":\"-webkit-border-before-style\",\"value\":\"none\"},{\"name\":\"-webkit-border-before-width\",\"value\":\"0px\"},{\"name\":\"-webkit-border-end-color\",\"value\":\"rgb(0, 0, 0)\"},{\"name\":\"-webkit-border-end-style\",\"value\":\"none\"},{\"name\":\"-webkit-border-end-width\",\"value\":\"0px\"},{\"name\":\"-webkit-border-horizontal-spacing\",\"value\":\"0px\"},{\"name\":\"-webkit-border-image\",\"value\":\"none\"},{\"name\":\"-webkit-border-start-color\",\"value\":\"rgb(0, 0, 0)\"},{\"name\":\"-webkit-border-start-style\",\"value\":\"none\"},{\"name\":\"-webkit-border-start-width\",\"value\":\"0px\"},{\"name\":\"-webkit-border-vertical-spacing\",\"value\":\"0px\"},{\"name\":\"-webkit-box-align\",\"value\":\"stretch\"},{\"name\":\"-webkit-box-decoration-break\",\"value\":\"slice\"},{\"name\":\"-webkit-box-direction\",\"value\":\"normal\"},{\"name\":\"-webkit-box-flex\",\"value\":\"0\"},{\"name\":\"-webkit-box-flex-group\",\"value\":\"1\"},{\"name\":\"-webkit-box-lines\",\"value\":\"single\"},{\"name\":\"-webkit-box-ordinal-group\",\"value\":\"1\"},{\"name\":\"-webkit-box-orient\",\"value\":\"horizontal\"},{\"name\":\"-webkit-box-pack\",\"value\":\"start\"},{\"name\":\"-webkit-box-reflect\",\"value\":\"none\"},{\"name\":\"-webkit-font-size-delta\",\"value\":\"\"},{\"name\":\"-webkit-highlight\",\"value\":\"none\"},{\"name\":\"-webkit-hyphenate-character\",\"value\":\"auto\"},{\"name\":\"-webkit-line-break\",\"value\":\"auto\"},{\"name\":\"-webkit-line-clamp\",\"value\":\"none\"},{\"name\":\"-webkit-logical-height\",\"value\":\"913px\"},{\"name\":\"-webkit-logical-width\",\"value\":\"1567px\"},{\"name\":\"-webkit-margin-after\",\"value\":\"8px\"},{\"name\":\"-webkit-margin-after-collapse\",\"value\":\"collapse\"},{\"name\":\"-webkit-margin-before\",\"value\":\"8px\"},{\"name\":\"-webkit-margin-before-collapse\",\"value\":\"collapse\"},{\"name\":\"-webkit-margin-bottom-collapse\",\"value\":\"collapse\"},{\"name\":\"-webkit-margin-end\",\"value\":\"8px\"},{\"name\":\"-webkit-margin-start\",\"value\":\"8px\"},{\"name\":\"-webkit-margin-top-collapse\",\"value\":\"collapse\"},{\"name\":\"-webkit-mask-box-image-outset\",\"value\":\"0px\"},{\"name\":\"-webkit-mask-box-image-repeat\",\"value\":\"stretch\"},{\"name\":\"-webkit-mask-box-image-slice\",\"value\":\"0 fill\"},{\"name\":\"-webkit-mask-box-image-source\",\"value\":\"none\"},{\"name\":\"-webkit-mask-box-image-width\",\"value\":\"auto\"},{\"name\":\"-webkit-mask-clip\",\"value\":\"border-box\"},{\"name\":\"-webkit-mask-composite\",\"value\":\"source-over\"},{\"name\":\"-webkit-mask-image\",\"value\":\"none\"},{\"name\":\"-webkit-mask-origin\",\"value\":\"border-box\"},{\"name\":\"-webkit-mask-position-x\",\"value\":\"0%\"},{\"name\":\"-webkit-mask-position-y\",\"value\":\"0%\"},{\"name\":\"-webkit-mask-repeat-x\",\"value\":\"\"},{\"name\":\"-webkit-mask-repeat-y\",\"value\":\"\"},{\"name\":\"-webkit-mask-size\",\"value\":\"auto\"},{\"name\":\"-webkit-max-logical-height\",\"value\":\"none\"},{\"name\":\"-webkit-max-logical-width\",\"value\":\"none\"},{\"name\":\"-webkit-min-logical-height\",\"value\":\"0px\"},{\"name\":\"-webkit-min-logical-width\",\"value\":\"0px\"},{\"name\":\"-webkit-padding-after\",\"value\":\"0px\"},{\"name\":\"-webkit-padding-before\",\"value\":\"0px\"},{\"name\":\"-webkit-padding-end\",\"value\":\"0px\"},{\"name\":\"-webkit-padding-start\",\"value\":\"0px\"},{\"name\":\"-webkit-perspective-origin-x\",\"value\":\"\"},{\"name\":\"-webkit-perspective-origin-y\",\"value\":\"\"},{\"name\":\"-webkit-print-color-adjust\",\"value\":\"economy\"},{\"name\":\"-webkit-rtl-ordering\",\"value\":\"logical\"},{\"name\":\"-webkit-ruby-position\",\"value\":\"before\"},{\"name\":\"-webkit-tap-highlight-color\",\"value\":\"rgba(0, 0, 0, 0.4)\"},{\"name\":\"-webkit-text-combine\",\"value\":\"none\"},{\"name\":\"-webkit-text-decorations-in-effect\",\"value\":\"none\"},{\"name\":\"-webkit-text-emphasis-color\",\"value\":\"rgb(0, 0, 0)\"},{\"name\":\"-webkit-text-emphasis-position\",\"value\":\"over right\"},{\"name\":\"-webkit-text-emphasis-style\",\"value\":\"none\"},{\"name\":\"-webkit-text-fill-color\",\"value\":\"rgb(0, 0, 0)\"},{\"name\":\"-webkit-text-security\",\"value\":\"none\"},{\"name\":\"-webkit-text-stroke-color\",\"value\":\"rgb(0, 0, 0)\"},{\"name\":\"-webkit-text-stroke-width\",\"value\":\"0px\"},{\"name\":\"-webkit-transform-origin-x\",\"value\":\"\"},{\"name\":\"-webkit-transform-origin-y\",\"value\":\"\"},{\"name\":\"-webkit-transform-origin-z\",\"value\":\"\"},{\"name\":\"-webkit-user-drag\",\"value\":\"auto\"},{\"name\":\"-webkit-user-modify\",\"value\":\"read-only\"},{\"name\":\"white-space\",\"value\":\"normal\"},{\"name\":\"widows\",\"value\":\"2\"},{\"name\":\"width\",\"value\":\"1567px\"},{\"name\":\"will-change\",\"value\":\"auto\"},{\"name\":\"word-break\",\"value\":\"normal\"},{\"name\":\"word-spacing\",\"value\":\"0px\"},{\"name\":\"word-wrap\",\"value\":\"normal\"},{\"name\":\"x\",\"value\":\"0px\"},{\"name\":\"y\",\"value\":\"0px\"},{\"name\":\"z-index\",\"value\":\"auto\"}]}}";

        Json::Value content;
        Json::Reader reader;
        reader.parse(style, content, false);
        return content["result"];
#endif
    }
    
    Json::Value InspectorCSSAgent::GetInlineStylesForNode(const Json::Value& params) {
        #ifndef INSPECTOR_TEST
        Json::Value root;
        Json::Value css_style;
        Json::Value computed_style = getInlineStyle(runtime_->render_tree_host()->render_root(), params["nodeId"].asInt64());
        css_style["cssProperties"] = computed_style;
        css_style["shorthandEntries"] = Json::Value(Json::arrayValue);
        root["inlineStyle"] = css_style;
        return root;
#else
        std::string style = "{\"id\":31,\"result\":{\"inlineStyle\":{\"styleSheetId\":\"38569.6\",\"cssProperties\":[],\"shorthandEntries\":[],\"cssText\":\"\",\"range\":{\"startLine\":0,\"startColumn\":0,\"endLine\":0,\"endColumn\":0}}}}";
        Json::Value content;
        Json::Reader reader;
        reader.parse(style, content, false);
        return content["result"];
#endif
    }
    
    Json::Value InspectorCSSAgent::CallMethod(Json::Value& content) {
        std::string method = content["method"].asString();
        Json::Value params = content["params"];
        
        auto iter = functions_map_.find(method);
        if(iter == functions_map_.end()) {
            Json::Value result;
            result["result"] = Json::Value(Json::objectValue);
            result["id"] = content["id"].asInt();
            return result;
        }
        
        Json::Value result;
        Json::Value root(Json::objectValue);
        
        result["result"] = (this->*(iter->second))(params);
        result["id"] = content["id"].asInt();
        
        return result;
    }
}
