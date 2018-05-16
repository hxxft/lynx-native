
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
        virtual ~VMContext();
        virtual void Initialize();
        virtual void Execute(const std::string& source);
        virtual Value Call(const std::string& name, const std::vector<Value>& args);
        virtual long GetParamsSize();
        virtual Value* GetParam(long index);
        virtual bool UpdateTopLevelVariable(const std::string &name, Value value);
    protected:
        friend class CodeGenerator;
        Heap& heap() {
            return heap_;
        }
    private:
        void Run();
        void RunFrame();
        bool CallFunction(Value* function, size_t argc, Value* ret);
        void GenerateClosure(Value* value, long index);
        Heap heap_;
        std::list<Frame> frames_;
    protected:
        friend class CodeGenerator;
        std::unordered_map<String*, long> top_level_variables_;
        base::ScopedPtr<Function> root_function_;
    };
}

#endif
