
#include "lepus/code_generator.h"
#include "lepus/guard.h"
#include "lepus/op_code.h"
#include "lepus/switch.h"

#include <stack>

namespace lepus {
    
#define MOVE(dst, src) \
            do { \
                if(dst >= 0) \
                    function->AddInstruction(Instruction::ABCode(TypeOp_Move, dst, src)); \
            } \
            while(0);
    
    
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
    
    void CodeGenerator::EnterLoop() {
        LoopGenerate* loop = new LoopGenerate;
        loop->loop_start_index_ = current_function_->function_->OpCodeSize();
        loop->parent_ = current_function_->current_loop_;
        current_function_->current_loop_.Reset(loop);
    }
    
    void CodeGenerator::LeaveLoop() {
        int end_loop_index = current_function_->function_->OpCodeSize();
        LoopGenerate* current_loop = current_function_->current_loop_.Get();
        for(size_t i = 0; i < current_loop->loop_infos_.size(); ++i) {
            LoopInfo* info = &current_loop->loop_infos_[i];
            int op_index = info->op_index_;
            if(info->type_ == LoopJmp_Tail) {
                current_function_->function_->GetInstruction(op_index)->RefillsBx(end_loop_index - op_index);
            }else if(info->type_ == LoopJmp_Head) {
                current_function_->function_->GetInstruction(op_index)->RefillsBx(current_loop->loop_start_index_ - op_index);
            }
        }
            
        current_function_->current_loop_ = current_loop->parent_;
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
        BlockGenerate* block = current->current_block_.Get();
        while(block) {
            auto iter = block->variables_map_.find(name);
            if(iter != block->variables_map_.end()) {
                return iter->second;
            }
            block = block->parent_.Get();
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
        int register_id = GenerateRegisiterId();
        ast->expression()->Accept(this, &register_id);
        auto instruction = Instruction::ACode(TypeOp_Ret, register_id);
        current_function_->function_->AddInstruction(instruction);
        ResetRegisiterId(register_id);
    }
    
    void CodeGenerator::WriteLocalValue(LexicalOp op, int dst, int src) {
        Function* function = current_function_->function_;
        Instruction instruction;
        switch (op) {
            case LexicalOp_Write:
                instruction = Instruction::ABCode(TypeOp_Move, dst, src);
                break;
            case LexicalOp_ASSIGN_ADD:
                instruction = Instruction::ABCCode(TypeOp_Add, dst, dst, src);
                break;
            case LexicalOp_ASSIGN_SUB:
                instruction = Instruction::ABCCode(TypeOp_Sub, dst, dst, src);
                break;
            case LexicalOp_ASSIGN_MUL:
                instruction = Instruction::ABCCode(TypeOp_Mul, dst, dst, src);
                break;
            case LexicalOp_ASSIGN_DIV:
                instruction = Instruction::ABCCode(TypeOp_Div, dst, dst, src);
                break;
            case LexicalOp_ASSIGN_MOD:
                instruction = Instruction::ABCCode(TypeOp_Mod, dst, dst, src);
                break;
            default:
                break;
        }
         function->AddInstruction(instruction);
    }
    
    void CodeGenerator::WriteUpValue(LexicalOp op, int dst, int src) {
        Function* function = current_function_->function_;
        Instruction instruction;
        int register_id = GenerateRegisiterId();
        switch (op) {
            case LexicalOp_ASSIGN_ADD:
                function->AddInstruction(Instruction::ABCode(TypeOp_GetUpvalue, register_id, dst));
                function->AddInstruction(Instruction::ABCCode(TypeOp_Add, src, register_id, src));
                break;
            case LexicalOp_ASSIGN_SUB:
                function->AddInstruction(Instruction::ABCode(TypeOp_GetUpvalue, register_id, dst));
                function->AddInstruction(Instruction::ABCCode(TypeOp_Sub, src, register_id, src));
                break;
            case LexicalOp_ASSIGN_MUL:
                function->AddInstruction(Instruction::ABCode(TypeOp_GetUpvalue, register_id, dst));
                function->AddInstruction(Instruction::ABCCode(TypeOp_Mul, src, register_id, src));
                break;
            case LexicalOp_ASSIGN_DIV:
                function->AddInstruction(Instruction::ABCode(TypeOp_GetUpvalue, register_id, dst));
                function->AddInstruction(Instruction::ABCCode(TypeOp_Div, src, register_id, src));
                break;
            case LexicalOp_ASSIGN_MOD:
                function->AddInstruction(Instruction::ABCode(TypeOp_GetUpvalue, register_id, dst));
                function->AddInstruction(Instruction::ABCCode(TypeOp_Mod, src, register_id, src));
                break;
            default:
                break;
        }
        function->AddInstruction(Instruction::ABCode(TypeOp_SetUpvalue, src, dst));
        ResetRegisiterId(register_id);
    }
    
    void CodeGenerator::AutomaticLocalValue(AutomaticType type, int dst, int src) {
        Function* function = current_function_->function_;
        switch (type) {
            case Automatic_None:
                MOVE(dst, src);
                break;
            case Automatic_Inc_Before:
                function->AddInstruction(Instruction::ACode(TypeOp_Inc, src));
                MOVE(dst, src);
                break;
            case Automatic_Inc_After:
                MOVE(dst, src);
                function->AddInstruction(Instruction::ACode(TypeOp_Inc, src));
                break;
            case Automatic_Dec_Before:
                MOVE(dst, src);
                function->AddInstruction(Instruction::ABCode(TypeOp_Move, dst, src));
                break;
            case Automatic_Dec_After:
                MOVE(dst, src);
                function->AddInstruction(Instruction::ACode(TypeOp_Dec, src));
                break;
            default:
                break;
        }
    }
    
    void CodeGenerator::AutomaticUpValue(AutomaticType type, int dst, int src) {
        Function* function = current_function_->function_;
        int register_id = GenerateRegisiterId();
        switch (type) {
            case Automatic_None:
                function->AddInstruction(Instruction::ABCode(TypeOp_GetUpvalue, dst, src));
                break;
            case Automatic_Inc_Before:
                function->AddInstruction(Instruction::ABCode(TypeOp_GetUpvalue, register_id, src));
                function->AddInstruction(Instruction::ACode(TypeOp_Inc, register_id));
                //function->AddInstruction(Instruction::ABCode(TypeOp_Move, dst, register_id));
                MOVE(dst, register_id);
                function->AddInstruction(Instruction::ABCode(TypeOp_SetUpvalue, register_id, src));
                break;
            case Automatic_Inc_After:
                function->AddInstruction(Instruction::ABCode(TypeOp_GetUpvalue, register_id, src));
                //function->AddInstruction(Instruction::ABCode(TypeOp_Move, dst, register_id));
                 MOVE(dst, register_id);
                function->AddInstruction(Instruction::ACode(TypeOp_Inc, register_id));
                function->AddInstruction(Instruction::ABCode(TypeOp_SetUpvalue, register_id, src));
                break;
            case Automatic_Dec_Before:
                function->AddInstruction(Instruction::ABCode(TypeOp_GetUpvalue, register_id, src));
                function->AddInstruction(Instruction::ACode(TypeOp_Dec, register_id));
                 MOVE(dst, register_id);
                //function->AddInstruction(Instruction::ABCode(TypeOp_Move, dst, register_id));
                function->AddInstruction(Instruction::ABCode(TypeOp_SetUpvalue, register_id, src));
                break;
            case Automatic_Dec_After:
                function->AddInstruction(Instruction::ABCode(TypeOp_GetUpvalue, register_id, src));
                 MOVE(dst, register_id);
                //function->AddInstruction(Instruction::ABCode(TypeOp_Move, dst, register_id));
                function->AddInstruction(Instruction::ACode(TypeOp_Dec, register_id));
                function->AddInstruction(Instruction::ABCode(TypeOp_SetUpvalue, register_id, src));
                break;
            default:
                break;
        }
        ResetRegisiterId(register_id);
    }
    
    void CodeGenerator::Visit(LiteralAST* ast, void* data){
        int register_id = data == nullptr ? -1 : *static_cast<int*>(data);
        Function* function = current_function_->function_;
        if(ast->lex_op() >= LexicalOp_Write) {
            if(ast->scope() == LexicalScoping_Local) {
                int var_reg_id = SearchVariable(ast->token().str_);
                WriteLocalValue(ast->lex_op(), var_reg_id, register_id);
            }else if(ast->scope() == LexicalScoping_Global) {
                
            }else if(ast->scope() == LexicalScoping_Upvalue) {
                int index = ManageUpvalues(ast->token().str_);
                WriteUpValue(ast->lex_op(), index, register_id);
            }
        }else if(ast->lex_op() == LexicalOp_Read){
            if(ast->token().token_ == Token_Number) {
                int index = function->AddConstNumber(ast->token().number_);
                auto instruction = Instruction::ABxCode(TypeOp_LoadConst, register_id, index);
                function->AddInstruction(instruction);
            }else if(ast->token().token_ == Token_Id) {
                if(ast->scope() == LexicalScoping_Local) {
                    int var_reg_id = SearchVariable(ast->token().str_);
//                    auto instruction = Instruction::ABCode(TypeOp_Move, register_id, var_reg_id);
//                    function->AddInstruction(instruction);
                    AutomaticLocalValue(ast->auto_type(), register_id, var_reg_id);
                }else if(ast->scope() == LexicalScoping_Global) {
                    int gloabl_index = SearchGlobal(ast->token().str_);
                    auto instruction = Instruction::ABxCode(TypeOp_GetGlobal, register_id, gloabl_index);
                    function->AddInstruction(instruction);
                    
                }else if(ast->scope() == LexicalScoping_Upvalue) {
                    int index = ManageUpvalues(ast->token().str_);
//                    Instruction instruction = Instruction::ABCode(TypeOp_GetUpvalue, register_id, index);
//                    function->AddInstruction(instruction);
                    AutomaticUpValue(ast->auto_type(), register_id, index);
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
            default:
                op_code = TypeOp_Move;
                break;
        }
        Instruction instruction;
        if(op_code != TypeOp_Move) {
            instruction = Instruction::ABCCode(op_code, register_id, left_register_id, right_register_id);
        }else{
            instruction = Instruction::ABCode(op_code, register_id, right_register_id);
        }
        
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
    
    void CodeGenerator::Visit(ForStatementAST* ast, void* data) {
        Guard<CodeGenerator> g(this, &CodeGenerator::EnterBlock, &CodeGenerator::LeaveBlock);
        int current_register = CurrentRegisiterId();
        
        if(ast->statement1().Get()) {
            ast->statement1()->Accept(this, nullptr);
        }
        
        Guard<CodeGenerator> l(this, &CodeGenerator::EnterLoop, &CodeGenerator::LeaveLoop);
        
        int jmp_index = -1;
        if(ast->statement2().Get()) {
            int register_id = GenerateRegisiterId();
            ast->statement2()->Accept(this, &register_id);
            Instruction instruction = Instruction::ABxCode(TypeOp_JmpFalse, register_id, 0);
            jmp_index = current_function_->function_->AddInstruction(instruction);
        }
        
        ast->block()->Accept(this, nullptr);
        
        for(base::ScopedVector<ASTree>::iterator iter = ast->statement3().begin();
            iter != ast->statement3().end(); ++iter) {
            (*iter)->Accept(this, nullptr);
        }
        

        Instruction instruction = Instruction::ABxCode(TypeOp_Jmp, 0, 0);
        int loop_head_index = current_function_->function_->AddInstruction(instruction);
        LoopGenerate* current_loop = current_function_->current_loop_.Get();
        if(current_loop) {
            current_loop->loop_infos_.push_back(LoopInfo(LoopJmp_Head, loop_head_index));
        }
        
        int end_index = current_function_->function_->OpCodeSize();
        if(jmp_index != -1) {
            current_function_->function_->GetInstruction(jmp_index)->RefillsBx(end_index - jmp_index);
        }
        
        ResetRegisiterId(current_register);
    }
    
    void CodeGenerator::Visit(DoWhileStatementAST* ast, void* data){
        Guard<CodeGenerator> g(this, &CodeGenerator::EnterLoop, &CodeGenerator::LeaveLoop);
        int current_register = CurrentRegisiterId();
        
        {
            Guard<CodeGenerator> g(this, &CodeGenerator::EnterBlock, &CodeGenerator::LeaveBlock);
            ast->block()->Accept(this, nullptr);
            
            int register_id = GenerateRegisiterId();
            ast->condition()->Accept(this, &register_id);
            Instruction jmp_false_instruction = Instruction::ABxCode(TypeOp_JmpFalse, register_id, 2);
            current_function_->function_->AddInstruction(jmp_false_instruction);
            
            Instruction instruction = Instruction::ABxCode(TypeOp_Jmp, 0, 0);
            int loop_head_index = current_function_->function_->AddInstruction(instruction);
            LoopGenerate* current_loop = current_function_->current_loop_.Get();
            if(current_loop) {
                current_loop->loop_infos_.push_back(LoopInfo(LoopJmp_Head, loop_head_index));
            }
        }
        
        ResetRegisiterId(current_register);
    }
    
    void CodeGenerator::Visit(BreakStatementAST* ast, void* data){
        Instruction instruction = Instruction::ABxCode(TypeOp_Jmp, 0, 0);
        int break_index = current_function_->function_->AddInstruction(instruction);
        LoopGenerate* current_loop = current_function_->current_loop_.Get();
        if(current_loop) {
            current_loop->loop_infos_.push_back(LoopInfo(LoopJmp_Tail, break_index));
        }
    }
    
    void CodeGenerator::Visit(WhileStatementAST* ast, void* data){
        Guard<CodeGenerator> g(this, &CodeGenerator::EnterLoop, &CodeGenerator::LeaveLoop);
        int register_id = GenerateRegisiterId();
        ast->condition()->Accept(this, &register_id);
        Instruction instruction = Instruction::ABxCode(TypeOp_JmpFalse, register_id, 0);
        int jmp_index = current_function_->function_->AddInstruction(instruction);
        {
            Guard<CodeGenerator> g(this, &CodeGenerator::EnterBlock, &CodeGenerator::LeaveBlock);
            ast->block()->Accept(this, nullptr);
            
            Instruction instruction = Instruction::ABxCode(TypeOp_Jmp, 0, 0);
            int loop_head_index = current_function_->function_->AddInstruction(instruction);
            LoopGenerate* current_loop = current_function_->current_loop_.Get();
            if(current_loop) {
                current_loop->loop_infos_.push_back(LoopInfo(LoopJmp_Head, loop_head_index));
            }
        }
        int end_index = current_function_->function_->OpCodeSize();
        current_function_->function_->GetInstruction(jmp_index)->RefillsBx(end_index - jmp_index);
        ResetRegisiterId(register_id);
    }
    
    void CodeGenerator::Visit(IfStatementAST* ast, void* data){
        int register_id = GenerateRegisiterId();
        ast->condition()->Accept(this, &register_id);
        Instruction instruction = Instruction::ABxCode(TypeOp_JmpFalse, register_id, 0);
        int jmp_index = current_function_->function_->AddInstruction(instruction);
        
        Guard<CodeGenerator> g(this, &CodeGenerator::EnterBlock, &CodeGenerator::LeaveBlock);
        
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
    
    void CodeGenerator::Visit(SwitchStatementAST* ast, void* data) {
        Guard<CodeGenerator> g(this, &CodeGenerator::EnterLoop, &CodeGenerator::LeaveLoop);
        int register_id = GenerateRegisiterId();
        ast->expression()->Accept(this, &register_id);
        SwitchInfo* info = SwitchInfo::Create(ast->cases());
        int index = current_function_->function_->AddSwitch(info);
        
        Instruction instruction = Instruction::ABxCode(TypeOp_Switch, register_id, index);
        int jmp_index = current_function_->function_->AddInstruction(instruction);
        
        for(base::ScopedVector<ASTree>::iterator iter = ast->cases().begin();
            iter != ast->cases().end(); ++iter) {
            Guard<CodeGenerator> g(this, &CodeGenerator::EnterBlock, &CodeGenerator::LeaveBlock);
            int case_jmp_index = current_function_->function_->OpCodeSize();
            (*iter)->Accept(this, &case_jmp_index);
            info->Modify(static_cast<CaseStatementAST*>(*iter)->key(), case_jmp_index-jmp_index);
        }
        ResetRegisiterId(register_id);
    }
    
    void CodeGenerator::Visit(CaseStatementAST* ast, void* data) {
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
        int current_register = CurrentRegisiterId();
        int return_register_id = data == nullptr ? GenerateRegisiterId() : *static_cast<int*>(data);
        int caller_register_id = GenerateRegisiterId();
        ast->caller()->Accept(this, &caller_register_id);
        ast->args()->Accept(this, nullptr);
        int argc = ast->args().Get() != nullptr ? static_cast<ExpressionListAST*>(ast->args().Get())->expressions().size() : 0;
        Instruction instruction = Instruction::ABCCode(TypeOp_Call, caller_register_id, argc, return_register_id);
        current_function_->function_->AddInstruction(instruction);
        ResetRegisiterId(current_register);
    }
}
