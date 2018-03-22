#ifndef LIBRARY_LOADER_LIB_MANAGER_H_
#define LIBRARY_LOADER_LIB_MANAGER_H_

#include <stack>
#include <map>
#include <string>
#include "base/macros.h"

namespace elf {
class ElfLibHandle; 
class ElfLibraryManager {
  public:
    ElfLibraryManager();
    ~ElfLibraryManager();
    void Add(ElfLibHandle* handle);
    void Release(ElfLibHandle* handle);
    void Delete(ElfLibHandle* handle);
    ElfLibHandle* Get(const std::string& name);
    void Clear();
  private:
    typedef std::map<std::string, ElfLibHandle*> LibsMap;
    typedef LibsMap::iterator LibIterator;
    LibsMap active_libs_;
    DISALLOW_COPY_AND_ASSIGN(ElfLibraryManager);
};
}

#endif