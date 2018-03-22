#include <unistd.h>
#include "library_loader/elf_loader.h"

#include "library_loader/lib_handle.h"
#include "library_loader/shared_library.h"
#include "library_loader/lib_manager.h"

namespace elf {
static ElfLoader* g_elf_loader = nullptr;

const Ehdr *Ehdr::Validate(const void *buf)
{
  if (!buf || buf == MAP_FAILED)
    return NULL;

  const Ehdr *ehdr = reinterpret_cast<const Ehdr *>(buf);

  /* Only support ELF executables or libraries for the host system */
  if (memcmp(ELFMAG, &ehdr->e_ident, SELFMAG) ||
      ehdr->e_ident[EI_CLASS] != ELFCLASS ||
      ehdr->e_ident[EI_DATA] != ELFDATA ||
      ehdr->e_ident[EI_VERSION] != 1 ||
      (ehdr->e_ident[EI_OSABI] != ELFOSABI && ehdr->e_ident[EI_OSABI] != ELFOSABI_NONE) ||
#ifdef EI_ABIVERSION
      ehdr->e_ident[EI_ABIVERSION] != ELFABIVERSION ||
#endif
      (ehdr->e_type != ET_EXEC && ehdr->e_type != ET_DYN) ||
      ehdr->e_machine != ELFMACHINE ||
      ehdr->e_version != 1 ||
      ehdr->e_phentsize != sizeof(Phdr))
    return nullptr;

  return ehdr;
}

ElfLoader* ElfLoader::Instance() {
  if (!g_elf_loader) {
    g_elf_loader = new ElfLoader();
  }
  return g_elf_loader;
}

ElfLoader::ElfLoader() : lib_manager_(new ElfLibraryManager) {}

ElfLoader::~ElfLoader() {
  delete lib_manager_;
}

void* ElfLoader::Load(const char* path, ElfLibType type) {

  std::string name(path);
  ElfLibHandle* handle = lib_manager_->Get(name);

  if (handle) {
    lib_manager_->Add(handle);
    return handle;
  }

  if (type == Elf_Lib_System) {
    SysElfLibHandle* sys_elf_handle = static_cast<SysElfLibHandle*>(handle);
    sys_elf_handle->Initialize();
    lib_manager_->Add(sys_elf_handle);
    return handle;
  }

  handle = ElfLibHandleFactory::Create(type, name, lib_manager_);

  if (type == Elf_Lib_Loaded &&
      !static_cast<LoadedElfLibHandle*>(handle)->DumpMemory()) {
    return nullptr;
  }

  SharedLibrary* lib = SharedLibrary::Create(path);
  LocalElfLibHandle* elf_handle = static_cast<LocalElfLibHandle*>(handle);
  LoadInner(lib, elf_handle);
  lib_manager_->Add(elf_handle);
  return handle;
}

bool ElfLoader::LoadInner(SharedLibrary* lib, LocalElfLibHandle* handle) {
  LibraryFirstPage page(lib);

  const Ehdr* ehdr = Ehdr::Validate(page.Address());
  if (!ehdr) {
    return false;
  }

  Addr min_vaddr = (Addr)-1;
  Addr max_vaddr = 0;
  std::vector<const Phdr*> pt_loads;
  const Phdr* pt_dyn = NULL;

  const Phdr* start_phdr = reinterpret_cast<const Phdr*>(
      reinterpret_cast<const char*>(ehdr) + ehdr->e_phoff);
  const Phdr* end_phdr = &start_phdr[ehdr->e_phnum];

  for (const Phdr* phdr = start_phdr; phdr < end_phdr; phdr++) {
    switch (phdr->p_type) {
      case PT_LOAD:
        pt_loads.push_back(phdr);
        if (phdr->p_vaddr < min_vaddr)
          min_vaddr = phdr->p_vaddr;
        if (max_vaddr < phdr->p_vaddr + phdr->p_memsz)
          max_vaddr = phdr->p_vaddr + phdr->p_memsz;
        break;
      case PT_DYNAMIC:
        pt_dyn = phdr;
        break;
      default:
        break;
    }
  }
  if (!handle->InitMemory(max_vaddr))
    return false;
  for (std::vector<const Phdr*>::iterator it = pt_loads.begin();
       it < pt_loads.end(); ++it)
    if (!LoadSegment(*it, lib, handle))
      return false;

  InitDynSegment(pt_dyn, lib, handle);

  handle->Init();
  return true;
}

// Init Dyn info and get depend libs
bool ElfLoader::InitDynSegment(const Phdr* phdr,
                               SharedLibrary* lib,
                               LocalElfLibHandle* handle) {
  const Dyn* first = reinterpret_cast<Dyn*>(handle->GetMemPtr(phdr->p_vaddr));
  const Dyn* last =
      reinterpret_cast<Dyn*>(handle->GetMemPtr(phdr->p_vaddr + phdr->p_filesz));
  std::vector<int> depends;  
  for (const Dyn* dyn = first; dyn < last && dyn->d_tag; dyn++) {
    switch (dyn->d_tag) {
      case DT_NEEDED:
        if (handle->lib_type() == Elf_Lib_Local) {
          depends.push_back(dyn->d_un.d_val);
        }
        break;
      case DT_PLTRELSZ:
        handle->jump_reloc_.InitSize(dyn->d_un.d_val);
        break;
      case DT_PLTGOT:
        break;
      case DT_HASH: {
        Word* hash_header =
            reinterpret_cast<Word*>(handle->GetMemPtr(dyn->d_un.d_ptr));
        handle->buckets_.InitBuffer(&hash_header[2]);
        handle->buckets_.InitSize(sizeof(Word) * hash_header[0]);
        handle->chains_.InitBuffer(reinterpret_cast<void*>(
            const_cast<Word*>((handle->buckets_.GetOffset(hash_header[0])))));
        // to-do check hash_header[1] || buckets_
      } break;
      case DT_STRTAB:
        handle->strtab_.InitBuffer(handle->GetMemPtr(dyn->d_un.d_ptr));
        break;
      case DT_SYMTAB:
        handle->symtab_.InitBuffer(handle->GetMemPtr(dyn->d_un.d_ptr));
        break;
      case DT_RELA:
        handle->reloc_.InitBuffer(handle->GetMemPtr(dyn->d_un.d_ptr));
        break;
      case DT_RELASZ:
        handle->reloc_.InitSize(dyn->d_un.d_val);
        break;
      case DT_RELAENT:
        if (dyn->d_un.d_val != sizeof(Reloc)) {
          return false;
        }
        break;
      case DT_STRSZ:
        handle->strtab_.InitSize(dyn->d_un.d_val);
        break;
      case DT_SYMENT:
        if (dyn->d_un.d_val != sizeof(Sym)) {
          return false;
        }
        break;
      case DT_INIT:
        handle->init_ = dyn->d_un.d_ptr;
        break;
      case DT_FINI:
        handle->fini_ = dyn->d_un.d_ptr;
        break;
      case DT_SONAME:
        break;
      case DT_RPATH:
        break;
      case DT_SYMBOLIC:
        break;
      case DT_REL:
        handle->reloc_.InitBuffer(handle->GetMemPtr(dyn->d_un.d_ptr));
        break;
      case DT_RELSZ:
        handle->reloc_.InitSize(dyn->d_un.d_val);
        break;
      case DT_RELENT:
        if (dyn->d_un.d_val != sizeof(Rel)) {
          return false;
        }
        break;
      case DT_PLTREL:
        break;
      case DT_JMPREL:
        handle->jump_reloc_.InitBuffer(handle->GetMemPtr(dyn->d_un.d_ptr));
        break;
      case DT_INIT_ARRAY:
        handle->init_array_.InitBuffer(handle->GetMemPtr(dyn->d_un.d_ptr));
        break;
      case DT_FINI_ARRAY:
        handle->fini_array_.InitBuffer(handle->GetMemPtr(dyn->d_un.d_ptr));
        break;
      case DT_INIT_ARRAYSZ:
        handle->init_array_.InitSize(dyn->d_un.d_val);
        break;
      case DT_FINI_ARRAYSZ:
        handle->fini_array_.InitSize(dyn->d_un.d_val);
        break;

      case DT_GNU_HASH: {
        handle->EnableGNUHash();
        Word* hash_header =
            reinterpret_cast<Word*>(handle->GetMemPtr(dyn->d_un.d_ptr));
        handle->hash_object_.nbuckets = hash_header[0];
        handle->hash_object_.symndx = hash_header[1];
        handle->hash_object_.maskwords = hash_header[2];
        handle->hash_object_.shift2 = hash_header[3];
        handle->hash_object_.bloom =
            reinterpret_cast<const BloomWord*>(&hash_header[4]);
        handle->hash_object_.buckets =
            &(handle->hash_object_.bloom[hash_header[2]]);
        handle->hash_object_.hashval =
            &(handle->hash_object_.buckets[hash_header[0]]);

      } break;
      default:
        break;
    }
  }
  for(int i = 0; i < depends.size(); ++i) {
    const char* name = handle->strtab_.GetOffset(depends[i]);
    ElfLibHandle* lib = static_cast<ElfLibHandle*>(ElfLoader::Instance()->Load(name, Elf_Lib_Local));
    handle->depend_libs()->push_back(lib);
  }
  return true;
}

bool ElfLoader::LoadSegment(const Phdr* phdr,
                                SharedLibrary* lib,
                                LocalElfLibHandle* handle) {
  int prot = SharedLibrary::Flags2Prot(phdr->p_flags);
  Addr align = (Addr)4069;  // PageSize;
  Addr align_offset;
  void *address, *mmap_addr;

  do {
    align_offset = phdr->p_vaddr - (phdr->p_vaddr & ~(align - 1));
    address = handle->GetMemPtr(phdr->p_vaddr - align_offset);
    mmap_addr =
        lib->Mmap(address, phdr->p_filesz + align_offset, prot,
                  MAP_PRIVATE | MAP_FIXED, phdr->p_offset - align_offset);
    if ((mmap_addr != MAP_FAILED) || (phdr->p_vaddr == 0) ||
        (phdr->p_align == align))
      break;
    align = phdr->p_align;
  } while (1);
  if (mmap_addr == MAP_FAILED)
    return false;
  return true;
}

void* ElfLoader::GetSymPtr(void* handle, const char* symbol) {
  void* symptr = nullptr;
  if (handle) {
    symptr = reinterpret_cast<ElfLibHandle*>(handle)->GetSymbolPtr(symbol);
  }
  return symptr;
}

void ElfLoader::Release(void* handle) {
  if (handle) {
    lib_manager_->Release(reinterpret_cast<ElfLibHandle*>(handle));
  }
}

}  // namespace elf