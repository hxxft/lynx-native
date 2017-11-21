
#ifndef LYNX_LEPUS_VM_CONTEXT_H_
#define LYNX_LEPUS_VM_CONTEXT_H_

#include <list>
#include <unordered_map>

#include "lepus/context.h"
#include "lepus/value.h"
#include "lepus/lepus_string.h"
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
        bool CallFunction(Value* function, int argc, Value* ret);
        void GenerateClosure(Value* value, int index);
        Heap heap_;
        std::list<Frame> frames_;
        std::unordered_map<String*, int> top_level_variables_;
    };
}

#endif
