#include "library_loader/elf_utils.h"
namespace elf {

void CallVoidFunction(void* ptr) {
  union {
    void* ptr;
    void (*func)(void);
  } f;

  f.ptr = ptr;
  f.func;
}

unsigned long SysVHash(const char* symbol) {
  const unsigned char* sym = reinterpret_cast<const unsigned char*>(symbol);
  unsigned long h = 0, g;
  while (*sym) {
    h = (h << 4) + *sym++;
    if ((g = h & 0xf0000000))
      h ^= g >> 24;
    h &= ~g;
  }
  return h;
}

unsigned long GNUHash(const char* symbol) {
  uint32_t h = 5381;

  for (; *symbol; symbol++) {
    h = (h << 5) + h + *symbol;
  }

  return h;
}

const char* LeafName(const char* path) {
  const char* lastSlash = strrchr(path, '/');
  if (lastSlash)
    return lastSlash + 1;
  return path;
}
}  // namespace elf
