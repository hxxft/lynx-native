// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_LOADER_LOADER_H_
#define LYNX_LOADER_LOADER_H_

#include <string>

#include "base/ref_counted_ptr.h"

namespace loader {
class LynxLoader : public base::RefCountPtr<LynxLoader> {
 public:
    enum FileType {
        MAIN_FILE,
        SCRIPT_FILE,
    };
    LynxLoader() {}
    virtual ~LynxLoader() {}
    virtual void Load(const std::string& url, int type) { }
    virtual void Flush() {}

 private:
    DISALLOW_COPY_AND_ASSIGN(LynxLoader);
};
}  // namespace loader

#endif  // LYNX_LOADER_LOADER_H_
