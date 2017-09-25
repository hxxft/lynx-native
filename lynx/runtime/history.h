// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_HISTORY_H_
#define LYNX_RUNTIME_HISTORY_H_

#include <vector>
#include <string>

#include "runtime/base/lynx_object_template.h"
#include "runtime/js_context.h"

namespace jscore {
    
class History : public LynxObjectTemplate {
    
public:
    History(JSContext* context);
    virtual ~History();
    
    void Load();
    void Back();
    void Forward();
    void Go(int offset);
    void Go(const std::string& url);
    void ReloadUrl(const std::string& url);
    
    int GetLength();
    
private:
    static base::ScopedPtr<LynxValue>
    ForwardCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

    static base::ScopedPtr<LynxValue>
    BackCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

    static base::ScopedPtr<LynxValue>
    GoCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

    static base::ScopedPtr<LynxValue> GetLengthCallback(LynxObjectTemplate* object);

    static const int kBlankPos = -1;
    
    std::vector<std::string> history_list_;
    JSContext* context_;
    int pos_;
};
    
}
#endif // LYNX_RUNTIME_HISTORY_H_
