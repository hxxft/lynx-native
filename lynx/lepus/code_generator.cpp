
#include "lepus/code_generator.h"
#include "lepus/guard.h"
#include "lepus/op_code.h"

#include <stack>

namespace lepus {
    
    void CodeGenerator::EnterFunction() {
        FunctionGenerate* function = new FunctionGenerate;
        function->parent_ = current_function_;
        current_function_.Reset(function);
        function->function_ = new Function;
         FunctionGenerate* parent = function->parent_.Get();
        if(parent) {
            function->function_->set_index(parent->function_->AddChildFunction(function->function_));
            
        }
    }
    
    void CodeGenerator::LeaveFunction() {
        base::ScopedPtr<FunctionGenerate> function_generate = current_function_;
        FunctionGenerate* parent = function_generate->parent_.Get();
        Function* function = function_generate->function_;
        current_function_ = function_generate->parent_;
        
        current_function_name_ = nullptr;
    }
    
    void CodeGenerator::EnterBlock() {
        BlockGenerate* block = new BlockGenerate;
        block->parent_ = current_function_->current_block_;
        current_function_->current_block_.Reset(block);
    }
    
    void CodeGenerator::LeaveBlock() {
        base::ScopedPtr<BlockGenerate> block = current_function_->current_block_;
        current_function_->current_block_ = block->parent_;
    }
    
    void CodeGenerator::InsertVariable(String* name, int register_id) {
        current_function_->current_block_->variables_map_[name] = register_id;
        if(current_function_->parent_.Get() == nullptr) {
            reinterpret_cast<std::unordered_map<String*, int>*>(top_level_variables_)->insert(std::make_pair(name, register_id));
        }
    }
    
    int CodeGenerator::SearchVariable(String* name) {
        return SearchVariable(name, current_function_.Get());
    }
    
    int CodeGenerator::SearchVariable(String* name, FunctionGenerate* current) {
        auto iter = current->current_block_->variables_map_.find(name);
        if(iter != current->current_block_->variables_map_.end()) {
            return iter->second;
        }
        return -1;
    }
    
    int CodeGenerator::SearchGlobal(String* name) {
        return context_->global().Search(name);
    }
    
    int CodeGenerator::ManageUpvalues(String* name) {
        Function* function = current_function_->function_;
        int index = function->SearchUpvalue(name);
        
        if(index > 0)
            return index;
        
        std::stack<FunctionGenerate*> parents;
        parents.push(current_function_->parent_.Get());
        
        int register_index = -1;
        bool in_parent_vars = false;
        
        while(!parents.empty()) {
            FunctionGenerate* current = parents.top();
            if(register_index >= 0) {
                int index = current->function_->AddUpvalue(name, register_index, in_parent_vars);
                in_parent_vars = false;
                register_index = index;
                parents.pop();
            }else{
                int register_id = SearchVariable(name, current);
                if(register_id >= 0) {
                    register_index = register_id;
                    in_parent_vars = true;
                    parents.pop();
                }else{
                    int index = current->function_->SearchUpvalue(name);
                    if(index >= 0) {
                        register_index = index;
                        in_parent_vars = false;
                        parents.pop();
                    }else{
                        parents.push(current->parent_.Get());
                    }
                }
            }
        }
        
        index = function->AddUpvalue(name, register_index, in_parent_vars);
        return index;
        
    }
    
    void CodeGenerator::Visit(ChunkAST* ast, void* data) {
        top_level_variables_ = data;
        Guard<CodeGenerator> g(this, &CodeGenerator::EnterFunction, &CodeGenerator::LeaveFunction);
        {
            Guard<CodeGenerator> g(this, &CodeGenerator::EnterBlock, &CodeGenerator::LeaveBlock);
            ast->block()->Accept(this, nullptr);
            Value* top = context_->heap().top_++;
            top->closure_ = new Closure(static_cast<Function*>(current_function_->function_));
            top->type_ = ValueT_Closure;
        }
    }
    
    void CodeGenerator::Visit(BlockAST* ast, void* data) {
        for(base::ScopedVector<ASTree>::iterator iter = ast->statements().begin();
            iter != ast->statements().end(); ++iter) {
            (*iter)->Accept(this, nullptr);
        }
        if(ast->return_statement().Get()) {
            ast->return_statement()->Accept(this, nullptr);
        }
    }
    
    void CodeGenerator::Visit(ReturnStatementAST* ast, void* data){
        
    }
    
    void CodeGenerator::Visit(LiteralAST* ast, void* data){
        int register_id = *static_cast<int*>(data);
        Function* function = current_function_->function_;
        if(ast->lex_op() == LexicalOp_Write) {
            if(ast->scope() == LexicalScoping_Local) {
                int var_reg_id = SearchVariable(ast->token().str_);
                auto instruction = Instruction::ABCode(TypeOp_Move, var_reg_id, register_id);
                function->AddInstruction(instruction);
            }else if(ast->scope() == LexicalScoping_Global) {
                
            }else if(ast->scope() == LexicalScoping_Upvalue) {
                int index = ManageUpvalues(ast->token().str_);
                Instruction instruction = Instruction::ABCode(TypeOp_SetUpvalue, register_id, index);
                function->AddInstruction(instruction);
            }
        }else if(ast->lex_op() == LexicalOp_Read){
            if(ast->token().token_ == Token_Number) {
                int index = function->AddConstNumber(ast->token().number_);
                auto instruction = Instruction::ABxCode(TypeOp_LoadConst, register_id, index);
                function->AddInstruction(instruction);
            }else if(ast->token().token_ == Token_Id) {
                if(ast->scope() == LexicalScoping_Local) {
                    int var_reg_id = SearchVariable(ast->token().str_);
                    auto instruction = Instruction::ABCode(TypeOp_Move, register_id, var_reg_id);
                    function->AddInstruction(instruction);
                }else if(ast->scope() == LexicalScoping_Global) {
                    int gloabl_index = SearchGlobal(ast->token().str_);
                    auto instruction = Instruction::ABCode(TypeOp_GetGlobal, register_id, gloabl_index);
                    function->AddInstruction(instruction);
                }else if(ast->scope() == LexicalScoping_Upvalue) {
                    int index = ManageUpvalues(ast->token().str_);
                    Instruction instruction = Instruction::ABCode(TypeOp_GetUpvalue, register_id, index);
                    function->AddInstruction(instruction);
                }
            }
        }
        
    }
    
    void CodeGenerator::Visit(NamesAST* ast, void* data){
        std::vector<Token>::iterator iter_name = ast->names().begin();
        for(;iter_name != ast->names().end(); ++iter_name) {
            InsertVariable(iter_name->str_, GenerateRegisiterId());
        }
    }
    
    void CodeGenerator::Visit(BinaryExprAST* ast, void* data){
        int register_id = *static_cast<int*>(data);
        Function* function = current_function_->function_;
        int left_register_id = GenerateRegisiterId();
        ast->left()->Accept(this, &left_register_id);
        ResetRegisiterId(left_register_id + 1);
        int right_register_id = GenerateRegisiterId();
        ast->right()->Accept(this, &right_register_id);
        ResetRegisiterId(right_register_id + 1);
        TypeOpCode op_code;
        switch (ast->op_token().token_) {
            case '+': op_code = TypeOp_Add; break;
            case '-': op_code = TypeOp_Sub; break;
            case '*': op_code = TypeOp_Mul; break;
            case '/': op_code = TypeOp_Div; break;
            case '^': op_code = TypeOp_Pow; break;
            case '%': op_code = TypeOp_Mod; break;
            case '<': op_code = TypeOp_Less; break;
            case '>': op_code = TypeOp_Greater; break;
            case Token_Equal: op_code = TypeOp_Equal; break;
            case Token_NotEqual: op_code = TypeOp_UnEqual; break;
            case Token_LessEqual: op_code = TypeOp_LessEqual; break;
            case Token_GreaterEqual: op_code = TypeOp_GreaterEqual; break;
            case Token_And: op_code = TypeOp_And; break;
            case Token_Or: op_code = TypeOp_Or; break;
            default: break;
        }
        Instruction instruction = Instruction::ABCCode(op_code, register_id, left_register_id, right_register_id);
        function->AddInstruction(instruction);
    }
    
    void CodeGenerator::Visit(UnaryExpression* ast, void* data){
        int register_id = *static_cast<int*>(data);
        Function* function = current_function_->function_;
        ast->expression()->Accept(this, data);
        TypeOpCode op_code;
        switch (ast->op_token().token_) {
            case '-':
                op_code = TypeOp_Neg;
                break;
            case '!':
                op_code = TypeOp_Not;
                break;
            default:
                break;
        }
        Instruction instruction = Instruction::ACode(op_code, register_id);
        function->AddInstruction(instruction);
    }
    
    void CodeGenerator::Visit(ExpressionListAST* ast, void* data){
        for(base::ScopedVector<ASTree>::iterator iter = ast->expressions().begin();
            iter != ast->expressions().end(); ++iter) {
            if(*iter) {
                int register_id = GenerateRegisiterId();
                (*iter)->Accept(this, &register_id);
            }
        }
    }
    
    void CodeGenerator::Visit(VariableAST* ast, void* data){
        int register_id = GenerateRegisiterId();
        InsertVariable(ast->identifier().str_, register_id);
        if(ast->expression().Get() == nullptr) {
            Instruction instruction = Instruction::ACode(TypeOp_LoadNil, register_id);
            current_function_->function_->AddInstruction(instruction);
        }else{
            ast->expression()->Accept(this, &register_id);
        }
        ResetRegisiterId(register_id + 1);
    }
    
    void CodeGenerator::Visit(VariableListAST* ast, void* data){
        for(base::ScopedVector<VariableAST>::iterator iter = ast->variable_list().begin();
            iter != ast->variable_list().end(); ++iter) {
            (*iter)->Accept(this, nullptr);
        }
    }
    
    void CodeGenerator::Visit(FunctionStatementAST* ast, void* data){
        int child_index = 0;
        int register_id = GenerateRegisiterId();
        {
            Guard<CodeGenerator> g(this, &CodeGenerator::EnterFunction, &CodeGenerator::LeaveFunction);
            {
                current_function_name_ = ast->function_name().str_;
                Guard<CodeGenerator> g(this, &CodeGenerator::EnterBlock, &CodeGenerator::LeaveBlock);
                ResetRegisiterId(0);
                child_index = current_function_->function_->index();
                if(ast->params().Get()) {
                    ast->params()->Accept(this, nullptr);
                }
                ast->body()->Accept(this, nullptr);
            }
        }
        ResetRegisiterId(register_id + 1);
        
        InsertVariable(ast->function_name().str_, register_id);
        Instruction i = Instruction::ABxCode(TypeOp_Closure,
                                             register_id,
                                             child_index);
        current_function_->function_->AddInstruction(i);
    }
    
    void CodeGenerator::Visit(BreakStatementAST* ast, void* data){
        
    }
    
    void CodeGenerator::Visit(WhileStatementAST* ast, void* data){
        
    }
    
    void CodeGenerator::Visit(IfStatementAST* ast, void* data){
        int register_id = GenerateRegisiterId();
        ast->condition()->Accept(this, &register_id);
        Instruction instruction = Instruction::ABxCode(TypeOp_JmpFalse, register_id, 0);
        Guard<CodeGenerator> g(this, &CodeGenerator::EnterBlock, &CodeGenerator::LeaveBlock);
        int jmp_index = current_function_->function_->AddInstruction(instruction);
        ast->true_branch()->Accept(this, nullptr);
        
        instruction = Instruction::ABxCode(TypeOp_Jmp, 0, 0);
        int true_jmp_index = current_function_->function_->AddInstruction(instruction);
        
        int index = current_function_->function_->OpCodeSize();
        current_function_->function_->GetInstruction(jmp_index)->RefillsBx(index - jmp_index);
        if(ast->false_branch().Get()) {
            ast->false_branch()->Accept(this, nullptr);
        }
        int end_index = current_function_->function_->OpCodeSize();
        current_function_->function_->GetInstruction(true_jmp_index)->RefillsBx(end_index - true_jmp_index);
        ResetRegisiterId(register_id);
    }
    
    void CodeGenerator::Visit(ElseStatementAST* ast, void* data){
        ast->block()->Accept(this, nullptr);
    }
    
    void CodeGenerator::Visit(AssignStatement* ast, void* data){
       int register_id = GenerateRegisiterId();
        //ast->assignment().token_
        ast->expression()->Accept(this, &register_id);
        ast->variable()->Accept(this, &register_id);
        ResetRegisiterId(register_id);
    }
    
    void CodeGenerator::Visit(MemberAccessorAST* ast, void* data){
        
    }
    
    void CodeGenerator::Visit(FunctionCallAST* ast, void* data){
        int register_id = GenerateRegisiterId();
        ast->caller()->Accept(this, &register_id);
        ast->args()->Accept(this, nullptr);
        int argc = ast->args().Get() != nullptr ? static_cast<ExpressionListAST*>(ast->args().Get())->expressions().size() : 0;
        Instruction instruction = Instruction::ABCCode(TypeOp_Call, register_id, argc, 0);
        current_function_->function_->AddInstruction(instruction);
        ResetRegisiterId(register_id);
    }
}
