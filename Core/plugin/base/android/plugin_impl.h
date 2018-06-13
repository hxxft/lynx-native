#ifndef LYNX_PLUGIN_BASE_ANDROID_PLUGIN_IMPL_H_
#define LYNX_PLUGIN_BASE_ANDROID_PLUGIN_IMPL_H_

#include "plugin/base/plugin.h"

#include "base/android/jni_helper.h"

namespace plugin {
class PluginImpl : public Plugin {
 public:
  PluginImpl();
  PluginImpl(const std::string& name);
  virtual ~PluginImpl();

  virtual void Exec(long client_id, base::ScopedPtr<jscore::LynxArray> args);
  virtual void AddEvent(const std::string& event);
  virtual void RemoveEvent(const std::string& event);

  static bool RegisterJNIUtils(JNIEnv* env);

  std::string name() { return name_; }

 private:
  base::android::ScopedGlobalJavaRef<jobject> plugin_impl_;
  std::string name_;
};
}  // namespace plugin

#endif