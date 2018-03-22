#include "library_loader/lib_manager.h"

#include "library_loader/lib_handle.h"

namespace elf {

ElfLibraryManager::ElfLibraryManager(){}

ElfLibraryManager::~ElfLibraryManager() {
  Clear();
}

void ElfLibraryManager::Add(ElfLibHandle* handle) {
  LibIterator iter = active_libs_.find(handle->GetName());
  if (iter == active_libs_.end()) {
    active_libs_.insert(std::make_pair(handle->GetName(), handle));
  }
  std::vector<ElfLibHandle*>* libs = handle->depend_libs();
  if(!libs->empty()) {
    for(int i = 0; i < libs->size(); ++i) {
      ElfLibHandle* lib = (*libs)[i];
      Add(lib);
    }
  }
  handle->AddRef(); 
}

void ElfLibraryManager::Release(ElfLibHandle* handle) {
  std::vector<ElfLibHandle*>* libs = handle->depend_libs();
  if(!libs->empty()) {
    for(int i = 0; i < libs->size(); ++i) {
      ElfLibHandle* lib = (*libs)[i];
      Release(lib);
    }
  }
  handle->Release(); 
} 

void ElfLibraryManager::Delete(ElfLibHandle* handle) {
  LibIterator iter = active_libs_.find(handle->GetName());
  if (iter == active_libs_.end())
    return;
  active_libs_.erase(iter);
}

ElfLibHandle* ElfLibraryManager::Get(const std::string& name) {
  LibIterator iter = active_libs_.find(name);
  if (iter == active_libs_.end())
    return nullptr;
  return iter->second;
}

void ElfLibraryManager::Clear() {
  std::vector<ElfLibHandle*> collector;
  for(LibIterator iter = active_libs_.begin(); iter != active_libs_.end(); ++iter) {
    collector.push_back(iter->second);
  }
  active_libs_.clear();
  for(int i = 0; i < collector.size(); ++i) {
    delete collector[i];
  }
}

}  // namespace elf