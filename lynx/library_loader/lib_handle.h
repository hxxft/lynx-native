#ifndef BASE_LIBRARY_LOADER_ELF_LIB_HANDLE_H_
#define BASE_LIBRARY_LOADER_ELF_LIB_HANDLE_H_

#include <dlfcn.h>
#include <sys/mman.h>
#include <vector>

#include "base/ref_counted_ptr.h"
#include "library_loader/user_elf.h"

namespace elf {
class ElfLibraryManager;

template <typename T>
class Array {
 public:
  const T* GetOffset(size_t offset) { return (buffer_ + offset); }
  size_t Size() { return size_; }
  void InitSize(size_t size) { size_ = size / sizeof(T); }
  void InitLength(size_t len) { size_ = len; }
  void InitBuffer(void* buffer) {
    buffer_ = reinterpret_cast<const T*>(buffer);
  }

 private:
  size_t size_;
  const T* buffer_;
};

class ElfLibHandle : public base::RefCountPtr<ElfLibHandle> {
 public:
  ElfLibHandle(ElfLibType type, const std::string& name, ElfLibraryManager* manager)
      : lib_name_(name),
        manager_(manager),
        lib_type_(type),
        is_gnu_hash_(false),
        depend_libs_() {}
  virtual ~ElfLibHandle();

  virtual void* GetSymbolPtr(const char* symbol) = 0;
  virtual void Init() {}
  virtual void Fini() {}

  void SetName(const std::string& name) { lib_name_ = name; }
  const std::string& GetName() { return lib_name_; }
  ElfLibType lib_type() { return lib_type_; }
  void EnableGNUHash() { is_gnu_hash_ = true; }

  std::vector<ElfLibHandle*>* depend_libs(){
    return &depend_libs_;
  } 

 protected:
  std::string lib_name_;
  ElfLibraryManager* manager_;
  ElfLibType lib_type_;
  bool is_gnu_hash_;
  std::vector<ElfLibHandle*> depend_libs_;  // DT_NEEDED
};

class SysElfLibHandle : public ElfLibHandle {
 public:
  SysElfLibHandle(ElfLibType type, const std::string& name, ElfLibraryManager* manager)
      : ElfLibHandle(type, name, manager), handle_(nullptr) {}

  virtual void* GetSymbolPtr(const char* symbol) {
    void* sym = dlsym(handle_, symbol);
    return sym;
  }

  virtual ~SysElfLibHandle() { dlclose(handle_); }

  bool Initialize() {
    if (lib_name_.empty()) {
      return false;
    }
    handle_ = dlopen(lib_name_.c_str(), RTLD_NOW);
    return true;
  }

 private:
  void* handle_;
};

class LocalElfLibHandle : public ElfLibHandle {
  friend class ElfLoader;

 public:
  LocalElfLibHandle(ElfLibType type, const std::string& name, ElfLibraryManager* manager)
      : ElfLibHandle(type, name, manager) {}
  virtual ~LocalElfLibHandle() {}
  virtual void* GetSymbolPtr(const char* symbol);
  virtual void Init();
  virtual void Fini();

  virtual bool InitMemory(size_t size) {
    memory_addr_ = mmap(NULL, size, PROT_NONE, MAP_SHARED | MAP_ANON, -1, 0);
    base_ = memory_addr_;
    size_ = size;
    // cout << "InitMemory: " << hex << base_ << " size:" << hex << size <<
    // endl;
    if (!base_)
      return false;
    return true;
  }

  void* GetMemPtr(const Addr offset) const {
    return reinterpret_cast<char*>(memory_addr_) + offset;
  }

 private:
  bool RelocateReloc();
  bool RelocateJumpReloc();

  void CallInit();
  void CallFini();

  void* GetRelocSymbolPtr(const char* symbol);
  void* GetSymbolPtrWithGNUHash(const char* symbol);
  void* GetSymbolPtrWithHash(const char* symbol);

 protected:
  void* base_;
  void* memory_addr_;
  unsigned long size_;

  bool initialized_;
  int name_;
  Addr init_;
  Addr fini_;
  Array<char> strtab_;
  Array<void*> init_array_, fini_array_;
  Array<Sym> symtab_;
  Array<Reloc> reloc_;
  Array<Reloc> jump_reloc_;

  Array<Word> buckets_;
  Array<Word> chains_;

  GNUHashObject hash_object_;
};

class LoadedElfLibHandle : public LocalElfLibHandle {
 public:
  LoadedElfLibHandle(ElfLibType type, const std::string& name, ElfLibraryManager* manager)
      : LocalElfLibHandle(type, name, manager) {}
  virtual ~LoadedElfLibHandle() {}
  virtual bool InitMemory(size_t size);
  virtual void Load() {}
  bool DumpMemory();
 private:
  typedef std::pair<Addr, Addr> MemoryRange;
  typedef std::map<std::string, MemoryRange> MemoryInfo; 
  MemoryInfo memory_info_;
};

class ElfLibHandleFactory {
 public:
  static ElfLibHandle* Create(ElfLibType type, const std::string& name, ElfLibraryManager* manager);
};

}  // namespace elf

#endif