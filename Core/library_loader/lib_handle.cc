#include "library_loader/lib_handle.h"

#include <base/print.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "library_loader/elf_loader.h"
#include "library_loader/elf_utils.h"
#include "library_loader/user_elf.h"
#include "library_loader/lib_manager.h"

namespace elf {

ElfLibHandle::~ElfLibHandle() {
  manager_->Delete(this);
  Fini();
}

void* LocalElfLibHandle::GetSymbolPtr(const char* symbol) {
  void* symptr = nullptr;

  if (is_gnu_hash_) {
    symptr = GetSymbolPtrWithGNUHash(symbol);
  } else {
    symptr = GetSymbolPtrWithHash(symbol);
  }

  if (lib_type() == Elf_Lib_Loaded) {
    symptr = reinterpret_cast<char*>(base_) +
             reinterpret_cast<unsigned long>(symptr) -
             reinterpret_cast<unsigned long>(memory_addr_);
  }

  return symptr;
}

void* LocalElfLibHandle::GetSymbolPtrWithHash(const char* symbol) {
  void* symptr = NULL;
  const Sym* sym = NULL;
  unsigned long hash = SysVHash(symbol);
  size_t bucket = hash % buckets_.Size();
  for (size_t y = *buckets_.GetOffset(bucket); y != STN_UNDEF;
       y = *chains_.GetOffset(y)) {
    if (strcmp(symbol, strtab_.GetOffset(symtab_.GetOffset(y)->st_name)) == 0) {
      sym = symtab_.GetOffset(y);
      break;
    }
  }
  if (sym && sym->st_shndx != SHN_UNDEF)
    symptr = GetMemPtr(sym->st_value);
  return symptr;
}

void* LocalElfLibHandle::GetSymbolPtrWithGNUHash(const char* symbol) {
  const uint32_t namehash = GNUHash(symbol);

  const uint32_t nbuckets = hash_object_.nbuckets;
  const uint32_t symoffset = hash_object_.symndx;
  const uint32_t bloom_size = hash_object_.maskwords;
  const uint32_t bloom_shift = hash_object_.shift2;
  const BloomWord* bloom = hash_object_.bloom;
  const uint32_t* buckets = (uint32_t*)&bloom[bloom_size];
  const uint32_t* chain = &buckets[nbuckets];

  uint32_t word = bloom[(namehash / ELFCLASS_BITS) % bloom_size];
  BloomWord mask = 0 | (BloomWord)1 << (namehash % ELFCLASS_BITS) |
                   (BloomWord)1 << ((namehash >> bloom_shift) % ELFCLASS_BITS);

  /* If at least one bit is not set, a symbol is surely missing. */
  if ((word & mask) != mask) {
    return NULL;
  }

  uint32_t symix = buckets[namehash % nbuckets];
  if (symix < symoffset) {
    return NULL;
  }

  /* Loop through the chain. */
  while (true) {
    const char* symname = strtab_.GetOffset(symtab_.GetOffset(symix)->st_name);
    const uint32_t hash = chain[symix - symoffset];

    if (namehash | 1 == hash | 1 && strcmp(symbol, symname) == 0) {
      void* symptr = NULL;
      const Sym* sym = symtab_.GetOffset(symix);
      if (sym && sym->st_shndx != SHN_UNDEF)
        symptr = GetMemPtr(sym->st_value);
      return symptr;
    }

    /* Chain ends with an element with the lowest bit set to 1. */
    if (hash & 1) {
      break;
    }

    symix++;
  }

  return nullptr;
}

void LocalElfLibHandle::CallFini() {
  if (fini_)
    CallVoidFunction(GetMemPtr(fini_));
  for (int i = 0; i < fini_array_.Size(); i++)
    CallVoidFunction(const_cast<void*>(*fini_array_.GetOffset(i)));
}

void LocalElfLibHandle::CallInit() {
  if (init_)
    CallVoidFunction(GetMemPtr(init_));
  for (int i = 0; i < init_array_.Size(); i++)
    CallVoidFunction(const_cast<void*>(*init_array_.GetOffset(i)));
}

void* LocalElfLibHandle::GetRelocSymbolPtr(const char* symbol) {
  void* symptr = NULL;
#ifdef __GLIBC__
  symptr = dlsym(RTLD_DEFAULT, symbol);
  if (symptr)
    return symptr;
#endif
  for (int i = 0; i < depend_libs_.size(); i++) {
    symptr = depend_libs_[i]->GetSymbolPtr(symbol);
    if (symptr)
      return symptr;
  }
}

bool LocalElfLibHandle::RelocateReloc() {
  for (int i = 0; i < reloc_.Size(); i++) {
    const Reloc* rel = reloc_.GetOffset(i);
    void* ptr = GetMemPtr(rel->r_offset);
    if (ELF_R_TYPE(rel->r_info) == R_RELATIVE) {
      *(void**)ptr = GetMemPtr(rel->GetAddend(base_));
      continue;
    }

    void* symptr = NULL;
    unsigned int symtab_index = ELF_R_SYM(rel->r_info);
    const Sym* sym = symtab_.GetOffset(symtab_index);
    if (sym->st_shndx != SHN_UNDEF) {
      symptr = GetMemPtr(sym->st_value);
    } else {
      symptr = GetRelocSymbolPtr(strtab_.GetOffset(sym->st_name));
    }

    switch (ELF_R_TYPE(rel->r_info)) {
      case R_GLOB_DAT:
        *(void**)ptr = symptr;
        break;
      case R_ABS:
        *(const char**)ptr = (const char*)symptr + rel->GetAddend(base_);
        break;
      default:
        break;
    }
  }
  return true;
}

bool LocalElfLibHandle::RelocateJumpReloc() {
  for (int i = 0; i < jump_reloc_.Size(); i++) {
    const Reloc* rel = jump_reloc_.GetOffset(i);
    void* ptr = GetMemPtr(rel->r_offset);
    if (ELF_R_TYPE(rel->r_info) != R_JMP_SLOT) {
      return false;
    }

    void* symptr = NULL;
    unsigned int symtab_index = ELF_R_SYM(rel->r_info);
    const Sym* sym = symtab_.GetOffset(symtab_index);

    if (sym->st_shndx != SHN_UNDEF)
      symptr = GetMemPtr(sym->st_value);
    else {
      symptr = GetRelocSymbolPtr(strtab_.GetOffset(sym->st_name));
    }
    *(void**)ptr = symptr;
  }
  return true;
}

void LocalElfLibHandle::Init() {
  RelocateReloc();
  RelocateJumpReloc();
  CallInit();
}

void LocalElfLibHandle::Fini() {
  CallFini();
}

bool LoadedElfLibHandle::InitMemory(size_t size) {
  size_ = size;
  memory_addr_ = mmap(NULL, size_, PROT_NONE, MAP_SHARED | MAP_ANON, -1, 0);
  MemoryInfo::iterator iter = memory_info_.find(lib_name_);
  if(iter != memory_info_.end()) {
    base_ = reinterpret_cast<void*>(iter->second.first);
    return true;
  }
  return false;
}

bool LoadedElfLibHandle::DumpMemory() {
  int pid = getpid();
  int buffer_size = 1024;
  char memory_map_file[buffer_size];
  FILE* fp = nullptr;
  sprintf(memory_map_file, "/proc/%d/maps", pid);
  if ((fp = fopen(memory_map_file, "r")) == nullptr) {
    return false;
  }

  char* buffer = memory_map_file;
  memset(buffer, 0, buffer_size);
  unsigned long start, end;
  char name[buffer_size];

  while (fgets(buffer, buffer_size, fp) != nullptr) {
    int rv =
        sscanf(buffer, "%08lx-%08lx %*s %*s %*s %*s %s\n", &start, &end, name);
    if (rv == 2)
      continue;
    
    std::string lib_name(name);
    MemoryInfo::iterator iter = memory_info_.find(std::string(lib_name));
    if(iter == memory_info_.end()) {
      memory_info_.insert(std::make_pair(
          lib_name, std::make_pair(start, end)));
    }else{
      iter->second.first = iter->second.first > start ? start : iter->second.first;
      iter->second.second = iter->second.second > end ? iter->second.second : end; 
    }
    memset(buffer, 0, buffer_size);
  }
  return !memory_info_.empty();
}

ElfLibHandle* ElfLibHandleFactory::Create(ElfLibType type, const std::string& name, ElfLibraryManager* manager) {
  switch (type) {
    case Elf_Lib_System:
      return new SysElfLibHandle(type, name, manager);
    case Elf_Lib_Local:
      return new LocalElfLibHandle(type, name, manager);
    case Elf_Lib_Loaded:
      return new LoadedElfLibHandle(type, name, manager);
    default:
      return nullptr;
  }
}

}  // namespace elf
