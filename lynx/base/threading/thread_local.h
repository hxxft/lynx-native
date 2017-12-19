#ifndef LYNX_BASE_THREADING_THREAD_LOCAL_H_
#define LYNX_BASE_THREADING_THREAD_LOCAL_H_

#include <pthread.h>

namespace base {
    namespace internal {
        class ThreadLocalPlatform {
        public:
            typedef pthread_key_t SlotType;
            
            static void AllocateSlot(SlotType* slot);
            static void FreeSlot(SlotType slot);
            static void* GetValueFromSlot(SlotType slot);
            static void SetValueInSlot(SlotType slot, void* value);
        };
    }
    
    template<typename Type>
    class ThreadLocalPointer {
    public:
        ThreadLocalPointer() : slot_() {
            internal::ThreadLocalPlatform::AllocateSlot(&slot_);
        }
        
        Type* Get() {
            return static_cast<Type*>(internal::ThreadLocalPlatform::GetValueFromSlot(slot_));
        }
        
        void Set(Type* ptr) {
            internal::ThreadLocalPlatform::SetValueInSlot(slot_, static_cast<void*>(ptr));
        }
        
        ~ThreadLocalPointer() {
            internal::ThreadLocalPlatform::FreeSlot(slot_);
        }
    private:
        typedef internal::ThreadLocalPlatform::SlotType SlotType;
        SlotType slot_;
    };
}

#endif  // LYNX_BASE_THREADING_THREAD_LOCAL_H_
