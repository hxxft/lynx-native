#ifndef BASE_LIBRARY_LOADER_DL_LOADER_H_
#define BASE_LIBRARY_LOADER_DL_LOADER_H_

#include "library_loader/user_elf.h"

void* lx_dlopen(const char* path, ElfLibType flags);

void* lx_dlsym(void* handle, const char* symbol);

void lx_dlclose(void* handle);

#endif