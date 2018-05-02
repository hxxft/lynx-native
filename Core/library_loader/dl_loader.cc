#include "library_loader/elf_loader.h"

using namespace elf;

void* lx_dlopen(const char* path, ElfLibType type) {
  return ElfLoader::Instance()->Load(path, type);
}

void* lx_dlsym(void* handle, const char* symbol) {
  return ElfLoader::Instance()->GetSymPtr(handle, symbol);
}

void lx_dlclose(void* handle) {
  ElfLoader::Instance()->Release(handle);
}
