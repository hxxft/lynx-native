#include "base/android/convert.h"
#include "base/android/java_type.h"
#include "base/threading/message_pump_android.h"
#include "render/android/render_object_impl_android.h"
#include "render/label_measurer.h"
#include "render/android/render_tree_host_impl_android.h"
#include "net/android/url_request_android.h"
#include "base/android/params_transform.h"
#include "runtime/android/jni_runtime_bridge.h"
#include "runtime/base/lynx_function_object_android.h"


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    base::android::InitVM(vm);
    JNIEnv* env = base::android::AttachCurrentThread();
    base::MessagePumpAndroid::RegisterJNIUtils(env);
    base::Convert::BindingJavaClass(env);
    lynx::LabelMeasurer::RegisterJNIUtils(env);
    net::URLRequestAndroid::RegisterJNIUtils(env);
    lynx::RenderObjectImplAndroid::RegisterJNIUtils(env);
    base::android::ParamsTransform::RegisterJNIUtils(env);
    jscore::JNIRuntimeBridge::RegisterJNIUtils(env);
    lynx::RenderTreeHostImplAndroid::RegisterJNIUtils(env);
    jscore::LynxFunctionObjectAndroid::RegisterJNIUtils(env);
    base::android::JType::Init(env, base::android::Type::LynxObject);
    base::android::JType::Init(env, base::android::Type::LynxArray);
    return JNI_VERSION_1_6;

}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
    //LOGD("lynx-error", "JNI_OnUnload");

}
