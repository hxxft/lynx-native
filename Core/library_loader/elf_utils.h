#ifndef BASE_LIBRARY_LOADER_ELF_UTILS_H_
#define BASE_LIBRARY_LOADER_ELF_UTILS_H_

#include "library_loader/user_elf.h"
#include "string.h"

namespace elf {

void CallVoidFunction(void* ptr);

unsigned long SysVHash(const char* symbol);

unsigned long GNUHash(const char* symbol);

const char* LeafName(const char* path);
}  // namespace elf
#endif
