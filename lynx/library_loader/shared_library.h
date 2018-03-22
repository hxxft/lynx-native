#ifndef BASE_LIBRARY_LOADER_SHARED_LIBRARY_H_
#define BASE_LIBRARY_LOADER_SHARED_LIBRARY_H_

#include <sys/mman.h>
#include "library_loader/user_elf.h"

namespace elf {
class SharedLibrary {
 public:
  ~SharedLibrary();
  static SharedLibrary* Create(const char* path);
  static int Flags2Prot(Word flags);
  void* Mmap(void* addr, size_t length, int prot, int flags, off_t offset) {
    return mmap(addr, length, prot, flags, fd_, offset);
  }
  static void Munmap(void* addr, size_t length) { munmap(addr, length); }

 protected:
  explicit SharedLibrary(int fd) : fd_(fd) {}

  int fd_;
};

class LibraryFirstPage {
 public:
  LibraryFirstPage(SharedLibrary* lib);
  ~LibraryFirstPage();
  void* Address() { return base_; }

 private:
  int PageSize() { return 4096; }
  void* base_;
};

}  // namespace elf

#endif