#include "runtime/jsc/js_value_ext.h"

bool JSValueIsArrayExt(JSContextRef ctx, JSValueRef value) {
    return JSValueIsArray(ctx, value);
}