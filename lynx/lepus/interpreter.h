#ifndef LYNX_LEPUS_INTERPRETER_H_
#define LYNX_LEPUS_INTERPRETER_H_

#include "lepus/visitor.h"
#include "lepus/value.h"
#include <unordered_map>

namespace lepus {
    class Interpreter : public Visitor{
    public:
        Interpreter():variables_map_(), parent_(nullptr){
            Initialize();
        }
        virtual ~Interpreter(){}
        virtual void Visit(ChunkAST* ast, void* data);
        virtual void Visit(BlockAST* ast, void* data);
        virtual void Visit(ReturnStatementAST* ast, void* data);
        virtual void Visit(LiteralAST* ast, void* data);
        virtual void Visit(NamesAST* ast, void* data);
        virtual void Visit(BinaryExprAST* ast, void* data);
        virtual void Visit(UnaryExpression* ast, void* data);
        virtual void Visit(ExpressionListAST* ast, void* data);
        virtual void Visit(VariableAST* ast, void* data);
        virtual void Visit(VariableListAST* ast, void* data);
        virtual void Visit(FunctionStatementAST* ast, void* data);
        virtual void Visit(BreakStatementAST* ast, void* data);
        virtual void Visit(WhileStatementAST* ast, void* data);
        virtual void Visit(IfStatementAST* ast, void* data);
        virtual void Visit(ElseStatementAST* ast, void* data);
        virtual void Visit(AssignStatement* ast, void* data);
        virtual void Visit(MemberAccessorAST* ast, void* data);
        virtual void Visit(FunctionCallAST* ast, void* data);
        
        void Call(const std::string& name, const std::vector<Value>& args);
        
        void set_parent(Interpreter* parent) {
            parent_ = parent;
        }
        
    private:
        void SearchVariable(const std::string& name, Value* value);
        void AssignVariable(const std::string& name, Value* value);
        void Initialize();
        
        std::unordered_map<std::string, Value> variables_map_;
        Interpreter* parent_;
    };
}

#endif  // LYNX_LEPUS_INTERPRETER_H_
