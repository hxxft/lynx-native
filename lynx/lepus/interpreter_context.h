#ifndef LYNX_LEPUS_INTERPRETER_CONTEXT_H_
#define LYNX_LEPUS_INTERPRETER_CONTEXT_H_

#include <string>
#include <vector>

#include "base/scoped_ptr.h"

#include "lepus/context.h"
#include "lepus/syntax_tree.h"
#include "lepus/interpreter.h"
#include "lepus/string.h"

namespace lepus {
    class InterpreterContext : public Context{
    public:
        InterpreterContext() {
            interpreter_.set_string_pool(&string_pool_);
        }
        virtual ~InterpreterContext(){}
        virtual void Initialize(){}
        virtual void Execute(const std::string& source);
        virtual Value Call(const std::string& name, const std::vector<Value>& args);
        virtual int GetParamsSize(){
            return 0;
        }
        virtual Value* GetParam(int index){
            return nullptr;
        }
    private:
        Interpreter interpreter_;
        base::ScopedPtr<ASTree> root_;
    };
}

#endif  // LYNX_LEPUS_INTERPRETER_VM_H_
