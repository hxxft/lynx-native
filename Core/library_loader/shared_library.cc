#include "library_loader/shared_library.h"

#include <fcntl.h>
#include <stdio.h>
#include "errno.h"

namespace elf {

SharedLibrary* SharedLibrary::Create(const char* path) {
  int fd = open(path, O_RDONLY);
  if (fd != -1)
    return new SharedLibrary(fd);
  return NULL;
}

int SharedLibrary::Flags2Prot(Word flags) {
  return ((flags & PF_X) ? PROT_EXEC : 0) | ((flags & PF_W) ? PROT_WRITE : 0) |
         ((flags & PF_R) ? PROT_READ : 0);
}

LibraryFirstPage::LibraryFirstPage(SharedLibrary* lib) {
  base_ = lib->Mmap(NULL, PageSize(), PROT_READ, MAP_PRIVATE, 0);
}

LibraryFirstPage::~LibraryFirstPage() {
  SharedLibrary::Munmap(base_, PageSize());
}


}  // namespace elf