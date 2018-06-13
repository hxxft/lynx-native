#include "content/content_main.h"

#include "plugin/plugin_server.h"
#include "content/lynx_thread.h"

#if OS_ANDROID

#include "LynxContent_jni.h"

void Initialize(JNIEnv* env, jobject jcaller) {
   content::ContentMain::Initialize();
}

bool content::ContentMain::RegisterContentMain(JNIEnv* env) {
  return RegisterNativesImpl(env);
}

#endif

namespace content {
    void ContentMain::Initialize() {
        content::LynxThread::Initialize();
        plugin::PluginServer::Initialize();
    }
}