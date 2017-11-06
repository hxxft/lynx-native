
#ifndef LYNX_LEPUS_VM_CONTEXT_H_
#define LYNX_LEPUS_VM_CONTEXT_H_

#include "lepus/context.h"
#include "lepus/value.h"
#include "lepus/string.h"
#include "lepus/heap.h"

namespace lepus {
    class VMContext : public Context {
    public:
        virtual ~VMContext(){}
        virtual void Initialize();
        virtual void Execute(const std::string& source);
        virtual Value Call(const std::string& name, const std::vector<Value>& args);
        virtual int GetParamsSize();
        virtual Value* GetParam(int index);
    protected:
        friend class CodeGenerator;
        Heap& heap() {
            return heap_;
        }
    private:
        void Run();
        void RunFrame();
        void CallFunction(Value* function, int argc, int result_count);
        Heap heap_;
        std::vector<Frame> frames_;
    };
}

#endif
