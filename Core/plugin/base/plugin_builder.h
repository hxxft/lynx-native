
#ifndef LYNX_PLUGIN_PLUGIN_BUILDER_H_
#define LYNX_PLUGIN_PLUGIN_BUILDER_H_

namespace plugin {
    class PluginManager;
    class PluginBuilder {
    public:
        static void build(PluginManager* manager);
    };
}

#endif
