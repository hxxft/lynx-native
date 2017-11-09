#ifndef LYNX_LEPUS_HEAP_H_
#define LYNX_LEPUS_HEAP_H_

#include "lepus/value.h"
#include "lepus/op_code.h"

namespace lepus {
    class Heap {
    public:
        Heap():heap_(kBaseHeapSize) {
            top_ = &heap_[0];
        }
        Value* top_;
        Value* base() {
            return &heap_[0];
        }
    private:
        static const int kBaseHeapSize = 30;
        std::vector<Value> heap_;
        
    };
    
    struct Frame {
        Value* register_;
        
        Value* function_;
        
        const Instruction* instruction_;
        
        const Instruction* end_;
        
        Frame() : register_(nullptr), function_(nullptr), instruction_(nullptr){}
    };
}

#endif
