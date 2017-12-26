
#include "lepus/code_generator.h"
#include "lepus/guard.h"
#include "lepus/op_code.h"
#include "lepus/switch.h"
#include "lepus/exception.h"

#include <stack>

namespace lepus {
    
#define NOOP \
    function->AddInstruction(Instruction::ACode(TypeOp_Noop, 0)); \

#define Load(type, reg, index) \
            do { \
                if(reg >= 0) \
                    function->AddInstruction(Instruction::ABxCode(type, reg, index)); \
            } \
            while(0);
    
#define Ret(reg) \
            do { \
                if(reg >= 0) \
                    function->AddInstruction(Instruction::ACode(TypeOp_Ret, reg)); \
            } \
            while(0);
    
#define MOVE(dst, src) \
            do { \
                if(src >= 0 && dst >= 0) \
                    function->AddInstruction(Instruction::ABCode(TypeOp_Move, dst, src)); \
            } \
            while(0);
    
#define Call(caller, argc, result) \
            do { \
                if(caller >= 0 && result >= 0 && argc >= 0) \
                    function->AddInstruction(Instruction::ABCCode(TypeOp_Call, caller, argc, result)); \
            } \
            while(0);
    
#define Switch(jmp_index, reg, index) \
            int jmp_index = -1; \
            do { \
                if(reg >= 0 && index >= 0) \
                    jmp_index = function->AddInstruction(Instruction::ABxCode(TypeOp_Switch, reg, index)); \
            } \
            while(0);
    
#define BinaryOperator(type, dst, src1, src2) \
            do { \
                if(src1 >= 0 && src2 >= 0 && dst >= 0) \
                    function->AddInstruction(Instruction::ABCCode(type, dst, src1, src2)); \
            } \
            while(0);
    
#define UpvalueOperator(type, reg, index) \
            do { \
                if(reg >= 0 && index >= 0) \
                    function->AddInstruction(Instruction::ABCode(type, reg, index)); \
            } \
            while(0);
 
#define AutomaticOperator(type, reg) \
            do { \
                if(reg >= 0) \
                    function->AddInstruction(Instruction::ACode(type, reg)); \
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
        for(std::size_t i = 0; i < current_loop->loop_infos_.size(); ++i) {
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
    
    void CodeGenerator::EnterRegister() {
        register_id_ = CurrentRegisiterId();
    }
    
    void CodeGenerator::LeaveRegister() {
        if(register_id_ > 0)
            ResetRegisiterId(register_id_);
        register_id_ = -1;
    }
    
    void CodeGenerator::InsertVariable(String* name, int register_id) {
        current_function_->current_block_->variables_map_[name] = register_id;
        if(current_function_->parent_.Get() == nullptr) {
            name->AddRef();
            context_->top_level_variables_.insert(std::make_pair(name, register_id));
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
        return context_->global()->Search(name);
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
        Guard<CodeGenerator> g(this, &CodeGenerator::EnterFunction, &CodeGenerator::LeaveFunction);
        {
            Guard<CodeGenerator> g(this, &CodeGenerator::EnterBlock, &CodeGenerator::LeaveBlock);
            ast->block()->Accept(this, nullptr);
            Value* top = context_->heap().top_++;
            top->closure_ = new Closure(static_cast<Function*>(current_function_->function_));
            top->type_ = Value_Closure;
        }
        context_->root_function_.Reset(static_cast<Function*>(current_function_->function_));
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
        Guard<CodeGenerator> g(this, &CodeGenerator::EnterRegister, &CodeGenerator::LeaveRegister);
        Function* function = current_function_->function_;
        int register_id = GenerateRegisiterId();
        if(ast->expression().Get())
            ast->expression()->Accept(this, &register_id);
        else
            Load(TypeOp_LoadNil, register_id, 0);
        Ret(register_id);
    }
    
    void CodeGenerator::WriteLocalValue(LexicalOp op, int dst, int src) {
        Function* function = current_function_->function_;
        Instruction instruction;
        switch (op) {
            case LexicalOp_Write:
                MOVE(dst, src);
                break;
            case LexicalOp_ASSIGN_ADD:
                BinaryOperator(TypeOp_Add, dst, dst, src);
                break;
            case LexicalOp_ASSIGN_SUB:
                BinaryOperator(TypeOp_Sub, dst, dst, src);
                break;
            case LexicalOp_ASSIGN_MUL:
                BinaryOperator(TypeOp_Mul, dst, dst, src);
                break;
            case LexicalOp_ASSIGN_DIV:
                BinaryOperator(TypeOp_Div, dst, dst, src);
                break;
            case LexicalOp_ASSIGN_MOD:
                BinaryOperator(TypeOp_Mod, dst, dst, src);
                break;
            default:
                break;
        }
    }
    
    void CodeGenerator::WriteUpValue(LexicalOp op, int dst, int src) {
        Guard<CodeGenerator> g(this, &CodeGenerator::EnterRegister, &CodeGenerator::LeaveRegister);
        Function* function = current_function_->function_;
        Instruction instruction;
        int register_id = GenerateRegisiterId();
        switch (op) {
            case LexicalOp_ASSIGN_ADD:
                UpvalueOperator(TypeOp_GetUpvalue, register_id, dst);
                BinaryOperator(TypeOp_Add, src, register_id, src);
                break;
            case LexicalOp_ASSIGN_SUB:
                UpvalueOperator(TypeOp_GetUpvalue, register_id, dst);
                BinaryOperator(TypeOp_Sub, src, register_id, src);
                break;
            case LexicalOp_ASSIGN_MUL:
                UpvalueOperator(TypeOp_GetUpvalue, register_id, dst);
                BinaryOperator(TypeOp_Mul, src, register_id, src);
                break;
            case LexicalOp_ASSIGN_DIV:
                UpvalueOperator(TypeOp_GetUpvalue, register_id, dst);
                BinaryOperator(TypeOp_Div, src, register_id, src);
                break;
            case LexicalOp_ASSIGN_MOD:
                UpvalueOperator(TypeOp_GetUpvalue, register_id, dst);
                BinaryOperator(TypeOp_Mod, src, register_id, src);
                break;
            default:
                break;
        }
        UpvalueOperator(TypeOp_SetUpvalue, src, dst);
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
                function->AddInstruction(Instruction::ACode(TypeOp_Dec, src));
                MOVE(dst, src);
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
        Guard<CodeGenerator> g(this, &CodeGenerator::EnterRegister, &CodeGenerator::LeaveRegister);
        Function* function = current_function_->function_;
        int register_id = GenerateRegisiterId();
        switch (type) {
            case Automatic_None:
                UpvalueOperator(TypeOp_GetUpvalue, dst, src);
                break;
            case Automatic_Inc_Before:
                UpvalueOperator(TypeOp_GetUpvalue, register_id, src);
                AutomaticOperator(TypeOp_Inc, register_id);
                MOVE(dst, register_id);
                UpvalueOperator(TypeOp_SetUpvalue, register_id, src);
                break;
            case Automatic_Inc_After:
                UpvalueOperator(TypeOp_GetUpvalue, register_id, src);
                MOVE(dst, register_id);
                AutomaticOperator(TypeOp_Inc, register_id);
                UpvalueOperator(TypeOp_SetUpvalue, register_id, src);
                break;
            case Automatic_Dec_Before:
                UpvalueOperator(TypeOp_GetUpvalue, register_id, src);
                AutomaticOperator(TypeOp_Dec, register_id);
                MOVE(dst, register_id);
                UpvalueOperator(TypeOp_SetUpvalue, register_id, src);
                break;
            case Automatic_Dec_After:
                UpvalueOperator(TypeOp_GetUpvalue, register_id, src);
                MOVE(dst, register_id);
                AutomaticOperator(TypeOp_Dec, register_id);
                UpvalueOperator(TypeOp_SetUpvalue, register_id, src);
                break;
            default:
                break;
        }
    }
    
    void CodeGenerator::Visit(LiteralAST* ast, void* data){
        int register_id = data == nullptr ? -1 : *static_cast<int*>(data);
        Function* function = current_function_->function_;
        if(ast->lex_op() >= LexicalOp_Write) {
            if(ast->scope() == LexicalScoping_Local) {
                int var_reg_id = SearchVariable(ast->token().str_);
                if(var_reg_id < 0) {
                    throw CompileException(ast->token().str_->c_str(), " is not defined", ast->token());
                }
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
            }else if(ast->token().token_ == Token_String){
                int index = function->AddConstString(ast->token().str_);
                auto instruction = Instruction::ABxCode(TypeOp_LoadConst, register_id, index);
                function->AddInstruction(instruction);
            }else if(ast->token().token_ == Token_True ||
                     ast->token().token_ == Token_False){
                int index = function->AddConstBoolean(ast->token().token_ == Token_True ? true : false);
                auto instruction = Instruction::ABxCode(TypeOp_LoadConst, register_id, index);
                function->AddInstruction(instruction);
            }else if(ast->token().token_ == Token_Id) {
                if(ast->scope() == LexicalScoping_Local) {
                    int var_reg_id = SearchVariable(ast->token().str_);
                    if(var_reg_id < 0) {
                        throw CompileException(ast->token().str_->c_str(), " is not defined", ast->token());
                    }
                    AutomaticLocalValue(ast->auto_type(), register_id, var_reg_id);
                }else if(ast->scope() == LexicalScoping_Global) {
                    int gloabl_index = SearchGlobal(ast->token().str_);
                    if(gloabl_index < 0) {
                        throw CompileException(ast->token().str_->c_str(), " is not defined", ast->token());
                    }
                    auto instruction = Instruction::ABxCode(TypeOp_GetGlobal, register_id, gloabl_index);
                    function->AddInstruction(instruction);
                    
                }else if(ast->scope() == LexicalScoping_Upvalue) {
                    int index = ManageUpvalues(ast->token().str_);
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
        switch (ast->op_token().token_) {
            case '+':
                BinaryOperator(TypeOp_Add, register_id, left_register_id, right_register_id);
                break;
            case '-':
                BinaryOperator(TypeOp_Sub, register_id, left_register_id, right_register_id);
                break;
            case '*':
                BinaryOperator(TypeOp_Mul, register_id, left_register_id, right_register_id);
                break;
            case '/':
                BinaryOperator(TypeOp_Div, register_id, left_register_id, right_register_id);
                break;
            case '^':
                BinaryOperator(TypeOp_Pow, register_id, left_register_id, right_register_id);
                break;
            case '%':
                BinaryOperator(TypeOp_Mod, register_id, left_register_id, right_register_id);
                break;
            case '<':
                BinaryOperator(TypeOp_Less, register_id, left_register_id, right_register_id);
                break;
            case '>':
                BinaryOperator(TypeOp_Greater, register_id, left_register_id, right_register_id);
                break;
            case Token_Equal:
                BinaryOperator(TypeOp_Equal, register_id, left_register_id, right_register_id);
                break;
            case Token_NotEqual:
                BinaryOperator(TypeOp_UnEqual, register_id, left_register_id, right_register_id);
                break;
            case Token_LessEqual:
                BinaryOperator(TypeOp_LessEqual, register_id, left_register_id, right_register_id);
                break;
            case Token_GreaterEqual:
                BinaryOperator(TypeOp_GreaterEqual, register_id, left_register_id, right_register_id);
                break;
            case Token_And:
                BinaryOperator(TypeOp_And, register_id, left_register_id, right_register_id);
                break;
            case Token_Or:
                BinaryOperator(TypeOp_Or, register_id, left_register_id, right_register_id);
                break;
            default:
                MOVE(register_id, right_register_id);
                break;
        }
    }
    
    void CodeGenerator::Visit(UnaryExpression* ast, void* data){
        int register_id = *static_cast<int*>(data);
        Function* function = current_function_->function_;
        ast->expression()->Accept(this, data);
        switch (ast->op_token().token_) {
            case '-':
                AutomaticOperator(TypeOp_Neg, register_id);
                break;
            case '!':
                AutomaticOperator(TypeOp_Not, register_id);
                break;
            default:
                NOOP;
                break;
        }
    }
    
    void CodeGenerator::Visit(ExpressionListAST* ast, void* data){
        for(base::ScopedVector<ASTree>::iterator iter = ast->expressions().begin();
            iter != ast->expressions().end(); ++iter) {
            if(*iter) {
                int register_id = GenerateRegisiterId();
                (*iter)->Accept(this, &register_id);
                ResetRegisiterId(register_id + 1);
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
        Function* function = current_function_->function_;
        Guard<CodeGenerator> r(this, &CodeGenerator::EnterRegister, &CodeGenerator::LeaveRegister);
        Guard<CodeGenerator> g(this, &CodeGenerator::EnterBlock, &CodeGenerator::LeaveBlock);
        
        if(ast->statement1().Get()) {
            ast->statement1()->Accept(this, nullptr);
        }
        
        Guard<CodeGenerator> l(this, &CodeGenerator::EnterLoop, &CodeGenerator::LeaveLoop);
        
        int jmp_index = -1;
        if(ast->statement2().Get()) {
            int register_id = GenerateRegisiterId();
            ast->statement2()->Accept(this, &register_id);
            Instruction instruction = Instruction::ABxCode(TypeOp_JmpFalse, register_id, 0);
            jmp_index = function->AddInstruction(instruction);
        }
        
        ast->block()->Accept(this, nullptr);
        
        for(base::ScopedVector<ASTree>::iterator iter = ast->statement3().begin();
            iter != ast->statement3().end(); ++iter) {
            (*iter)->Accept(this, nullptr);
        }
        

        Instruction instruction = Instruction::ABxCode(TypeOp_Jmp, 0, 0);
        int loop_head_index = function->AddInstruction(instruction);
        LoopGenerate* current_loop = current_function_->current_loop_.Get();
        if(current_loop) {
            current_loop->loop_infos_.push_back(LoopInfo(LoopJmp_Head, loop_head_index));
        }
        
        int end_index = function->OpCodeSize();
        if(jmp_index != -1) {
            function->GetInstruction(jmp_index)->RefillsBx(end_index - jmp_index);
        }
    }
    
    void CodeGenerator::Visit(DoWhileStatementAST* ast, void* data){
        Function* function = current_function_->function_;
        Guard<CodeGenerator> g(this, &CodeGenerator::EnterRegister, &CodeGenerator::LeaveRegister);
        {
            Guard<CodeGenerator> g(this, &CodeGenerator::EnterLoop, &CodeGenerator::LeaveLoop);
            {
                Guard<CodeGenerator> g(this, &CodeGenerator::EnterBlock, &CodeGenerator::LeaveBlock);
                ast->block()->Accept(this, nullptr);
                
                int register_id = GenerateRegisiterId();
                ast->condition()->Accept(this, &register_id);
                Instruction jmp_false_instruction = Instruction::ABxCode(TypeOp_JmpFalse, register_id, 2);
                function->AddInstruction(jmp_false_instruction);
                
                Instruction instruction = Instruction::ABxCode(TypeOp_Jmp, 0, 0);
                int loop_head_index = function->AddInstruction(instruction);
                LoopGenerate* current_loop = current_function_->current_loop_.Get();
                if(current_loop) {
                    current_loop->loop_infos_.push_back(LoopInfo(LoopJmp_Head, loop_head_index));
                }
            }
        }
    }
    
    void CodeGenerator::Visit(BreakStatementAST* ast, void* data){
        Function* function = current_function_->function_;
        Instruction instruction = Instruction::ABxCode(TypeOp_Jmp, 0, 0);
        int break_index = function->AddInstruction(instruction);
        LoopGenerate* current_loop = current_function_->current_loop_.Get();
        if(current_loop) {
            current_loop->loop_infos_.push_back(LoopInfo(LoopJmp_Tail, break_index));
        }
    }
    
    void CodeGenerator::Visit(WhileStatementAST* ast, void* data){
        Function* function = current_function_->function_;
        Guard<CodeGenerator> g(this, &CodeGenerator::EnterRegister, &CodeGenerator::LeaveRegister);
        {
            Guard<CodeGenerator> g(this, &CodeGenerator::EnterLoop, &CodeGenerator::LeaveLoop);
            int register_id = GenerateRegisiterId();
            ast->condition()->Accept(this, &register_id);
            Instruction instruction = Instruction::ABxCode(TypeOp_JmpFalse, register_id, 0);
            int jmp_index = function->AddInstruction(instruction);
            {
                Guard<CodeGenerator> g(this, &CodeGenerator::EnterBlock, &CodeGenerator::LeaveBlock);
                ast->block()->Accept(this, nullptr);
                
                Instruction instruction = Instruction::ABxCode(TypeOp_Jmp, 0, 0);
                int loop_head_index = function->AddInstruction(instruction);
                LoopGenerate* current_loop = current_function_->current_loop_.Get();
                if(current_loop) {
                    current_loop->loop_infos_.push_back(LoopInfo(LoopJmp_Head, loop_head_index));
                }
            }
            int end_index = function->OpCodeSize();
            function->GetInstruction(jmp_index)->RefillsBx(end_index - jmp_index);
        }
    }
    
    void CodeGenerator::Visit(IfStatementAST* ast, void* data){
        Function* function = current_function_->function_;
        Guard<CodeGenerator> g(this, &CodeGenerator::EnterRegister, &CodeGenerator::LeaveRegister);
        {
            int register_id = GenerateRegisiterId();
            ast->condition()->Accept(this, &register_id);
            Instruction instruction = Instruction::ABxCode(TypeOp_JmpFalse, register_id, 0);
            int jmp_index = function->AddInstruction(instruction);
            
            Guard<CodeGenerator> g(this, &CodeGenerator::EnterBlock, &CodeGenerator::LeaveBlock);
            
            ast->true_branch()->Accept(this, nullptr);
            
            instruction = Instruction::ABxCode(TypeOp_Jmp, 0, 0);
            int true_jmp_index = function->AddInstruction(instruction);
            
            int index = function->OpCodeSize();
            function->GetInstruction(jmp_index)->RefillsBx(index - jmp_index);
            if(ast->false_branch().Get()) {
                ast->false_branch()->Accept(this, nullptr);
            }
            int end_index = function->OpCodeSize();
            function->GetInstruction(true_jmp_index)->RefillsBx(end_index - true_jmp_index);
        }
    }
    
    void CodeGenerator::Visit(ElseStatementAST* ast, void* data){
        ast->block()->Accept(this, nullptr);
    }
    
    void CodeGenerator::Visit(SwitchStatementAST* ast, void* data) {
        Function* function = current_function_->function_;
        Guard<CodeGenerator> g(this, &CodeGenerator::EnterRegister, &CodeGenerator::LeaveRegister);
        {
            Guard<CodeGenerator> g(this, &CodeGenerator::EnterLoop, &CodeGenerator::LeaveLoop);
            int register_id = GenerateRegisiterId();
            ast->expression()->Accept(this, &register_id);
            SwitchInfo* info = SwitchInfo::Create(ast->cases());
            int index = function->AddSwitch(info);
            Switch(jmp_index, register_id, index);
            for(base::ScopedVector<ASTree>::iterator iter = ast->cases().begin();
                iter != ast->cases().end(); ++iter) {
                Guard<CodeGenerator> g(this, &CodeGenerator::EnterBlock, &CodeGenerator::LeaveBlock);
                int case_jmp_index = function->OpCodeSize();
                (*iter)->Accept(this, &case_jmp_index);
                info->Modify(static_cast<CaseStatementAST*>(*iter)->key(), case_jmp_index-jmp_index);
            }
            if(info->default_offset() == -1) {
                info->set_default_offset(function->OpCodeSize() - jmp_index);
            }
        }
    }
    
    void CodeGenerator::Visit(CaseStatementAST* ast, void* data) {
        ast->block()->Accept(this, nullptr);
    }
    
    void CodeGenerator::Visit(AssignStatement* ast, void* data){
        Guard<CodeGenerator> g(this, &CodeGenerator::EnterRegister, &CodeGenerator::LeaveRegister);
        int register_id = GenerateRegisiterId();
        ast->expression()->Accept(this, &register_id);
        ast->variable()->Accept(this, &register_id);
    }
    
    void CodeGenerator::Visit(MemberAccessorAST* ast, void* data){
        Guard<CodeGenerator> g(this, &CodeGenerator::EnterRegister, &CodeGenerator::LeaveRegister);
        Function* function = current_function_->function_;
        
        int reg = GenerateRegisiterId();
        ast->table()->Accept(this, &reg);
        
        int member_reg_id = GenerateRegisiterId();
        int index = function->AddConstString(ast->member().str_);

        function->AddInstruction(Instruction::ABxCode(TypeOp_LoadConst, member_reg_id, index));
        
        function->AddInstruction(Instruction::ABCCode(TypeOp_GetTable, *static_cast<int*>(data), reg, member_reg_id));
    }
    
    void CodeGenerator::Visit(FunctionCallAST* ast, void* data){
        Guard<CodeGenerator> g(this, &CodeGenerator::EnterRegister, &CodeGenerator::LeaveRegister);
        Function* function = current_function_->function_;
        int return_register_id = data == nullptr ? GenerateRegisiterId() : *static_cast<int*>(data);
        int caller_register_id = GenerateRegisiterId();
        ast->caller()->Accept(this, &caller_register_id);
        
        int argc = ast->args().Get() != nullptr ? static_cast<ExpressionListAST*>(ast->args().Get())->expressions().size() : 0;
        
        if(ast->caller()->type() == ASTType_MemberAccessor) {
            int this_reg_id = GenerateRegisiterId();
            static_cast<MemberAccessorAST*>(ast->caller().Get())->table()->Accept(this, &this_reg_id);
            ++argc;
        }
        
        ast->args()->Accept(this, nullptr);
        
        Call(caller_register_id, argc, return_register_id);
    }
    void CodeGenerator::Visit(TernaryStatementAST *ast, void *data){
        int return_register_id = *static_cast<int*>(data);
        Function* function = current_function_->function_;
        int condition_register_id = GenerateRegisiterId();
        ResetRegisiterId(condition_register_id + 1);
        
        ast->condition()->Accept(this, &condition_register_id);
        Instruction instruction = Instruction::ABxCode(TypeOp_JmpFalse, condition_register_id, 0);
        int jmp_index = function->AddInstruction(instruction);
        
        ast->true_branch()->Accept(this, &return_register_id);
        instruction = Instruction::ABxCode(TypeOp_Jmp, return_register_id, 0);
        int true_jmp_index = function->AddInstruction(instruction);
        int index = function->OpCodeSize();
        function->GetInstruction(jmp_index)->RefillsBx(index - jmp_index);
        
        ast->false_branch()->Accept(this, &return_register_id);
        int end_index = function->OpCodeSize();
        function->GetInstruction(true_jmp_index)->RefillsBx(end_index - true_jmp_index);
    }
}
