#ifndef BASE_LIBRARY_LOADER_ELF_LOADER_H_
#define BASE_LIBRARY_LOADER_ELF_LOADER_H_

#include <stack>
#include <string>
#include <vector>
#include <map>
#include "base/macros.h"

#include "library_loader/user_elf.h"

namespace elf {
class ElfLibHandle;
class LocalElfLibHandle;
class LoadedElfLibHandle;
class SharedLibrary;
class ElfLibraryManager;

class ElfLoader {
 public:
  static ElfLoader* Instance();
  ElfLoader();
  ~ElfLoader();
  void* Load(const char* path, ElfLibType type);
  void* GetSymPtr(void* handle, const char* symbol);
  void Release(void* handle);

 protected:
  typedef std::vector<ElfLibHandle*>::iterator LibIterator;
  friend class ElfLibHandle;
  friend class LoadedElfLibHandle;

  bool LoadInner(SharedLibrary* lib, LocalElfLibHandle* handle);

  bool LoadSegment(const Phdr* phdr,
                       SharedLibrary* lib,
                       LocalElfLibHandle* handle);
  bool InitDynSegment(const Phdr* dyn,
                      SharedLibrary* lib,
                      LocalElfLibHandle* handle);




  ElfLibraryManager* lib_manager_;
  private:
  DISALLOW_COPY_AND_ASSIGN(ElfLoader);
};

}  // namespace elf

#endif