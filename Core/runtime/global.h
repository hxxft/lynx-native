// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_GLOBAL_H_
#define LYNX_RUNTIME_GLOBAL_H_

#include "runtime/base/lynx_object.h"

namespace plugin {
class PluginManager;
}

namespace jscore {
class Console;
class Navigator;
class Screen;
class Loader;
class Document;

class Global : public LynxObject {
 public:
  Global(JSContext* context);
  virtual ~Global();
  void OnJSObjectAttached();
  inline Console* console() { return console_.Get(); }
  inline Navigator* navigator() { return navigator_.Get(); }
  inline Screen* screen() { return screen_.Get(); }
  inline Loader* loader() { return loader_.Get(); }
  inline Document* document() { return document_.Get(); }
#if PLUGIN_ENABLE
  inline plugin::PluginManager* plugin() { return plugin_.Get(); }
#endif

  base::ScopedPtr<LynxValue> SetTimeout(base::ScopedPtr<LynxArray>& array);
  base::ScopedPtr<LynxValue> SetInterval(base::ScopedPtr<LynxArray>& array);
  base::ScopedPtr<LynxValue> ClearTimeout(base::ScopedPtr<LynxArray>& array);
  base::ScopedPtr<LynxValue> ClearInterval(base::ScopedPtr<LynxArray>& array);

  base::ScopedPtr<LynxValue> GetDevicePixelRatio();
  base::ScopedPtr<LynxValue> GetConsole();
  base::ScopedPtr<LynxValue> GetNavigator();
  base::ScopedPtr<LynxValue> GetScreen();
  base::ScopedPtr<LynxValue> GetLoader();
  base::ScopedPtr<LynxValue> GetDocument();
  base::ScopedPtr<LynxValue> GetPlugin();

 private:
  base::ScopedRefPtr<Console> console_;
  base::ScopedRefPtr<Navigator> navigator_;
  base::ScopedRefPtr<Screen> screen_;
  base::ScopedRefPtr<Loader> loader_;
  base::ScopedRefPtr<Document> document_;
#if PLUGIN_ENABLE
  base::ScopedRefPtr<plugin::PluginManager> plugin_;
#endif
  DISALLOW_COPY_AND_ASSIGN(Global);
};
}  // namespace jscore

#endif  // LYNX_RUNTIME_GLOBAL_H_
