#ifndef LYNX_LEPUS_INTERPRETER_H_
#define LYNX_LEPUS_INTERPRETER_H_

#include "lepus/visitor.h"
#include "lepus/value.h"
#include "lepus/string.h"
#include <unordered_map>

namespace lepus {
    typedef Value (*NativeFunction)(const std::vector<Value>& args);
    class Interpreter : public Visitor{
    public:
        Interpreter()
        : string_pool_(nullptr),
          variables_map_(),
          parent_(nullptr){
//            Initialize();
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
        virtual void Visit(DoWhileStatementAST* ast, void* data);
        virtual void Visit(ForStatementAST* ast, void* data);
        virtual void Visit(BreakStatementAST* ast, void* data);
        virtual void Visit(WhileStatementAST* ast, void* data);
        virtual void Visit(IfStatementAST* ast, void* data);
        virtual void Visit(ElseStatementAST* ast, void* data);
        virtual void Visit(SwitchStatementAST* ast, void* data);
        virtual void Visit(CaseStatementAST* ast, void* data);
        virtual void Visit(AssignStatement* ast, void* data);
        virtual void Visit(MemberAccessorAST* ast, void* data);
        virtual void Visit(FunctionCallAST* ast, void* data);
        
        void Call(const String* name, const std::vector<Value>& args);
        
        void set_parent(Interpreter* parent) {
            parent_ = parent;
        }
        
        void set_string_pool(StringPool* string_pool) {
            string_pool_ = string_pool;
        }
        void Initialize();
    private:
        enum ValueOp
        {
            ValueOp_None,
            ValueOp_Read,
            ValueOp_Write,
        };
        struct ValueData {
            Value value_;
            ValueOp op_;
            int assigment_;
            ValueData():value_(), op_(ValueOp_Read),assigment_(-1) {
                
            }
        };
        void SearchVariable(const String* name, ValueData* value);
        void AssignVariable(const String* name, ValueData* value);
        
        
        StringPool* string_pool_;
        std::unordered_map<String*, Value> variables_map_;
        Interpreter* parent_;
    };
}

#endif  // LYNX_LEPUS_INTERPRETER_H_
