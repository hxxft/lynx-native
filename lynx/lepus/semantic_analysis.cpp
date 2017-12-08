
#include "lepus/semantic_analysis.h"

#include "lepus/exception.h"
#include "lepus/guard.h"

namespace lepus {
    
    enum ExprType
    {
        ExprType_Unknown,    // unknown type at semantic analysis phase
        ExprType_Nil,
        ExprType_Bool,
        ExprType_Number,
        ExprType_String,
        ExprType_Table,
    };
    
    struct ExprData {
        ExprType expr_type_;
        LexicalOp lex_po_;
        ExprData()
            :expr_type_(ExprType_Unknown),
            lex_po_(LexicalOp_None){
                
            }
    };
    
    void SemanticAnalysis::EnterFunction() {
        LexicalFunction* function = new LexicalFunction;
        function->parent_ = current_function_;
        current_function_.Reset(function);
    }
    
    void SemanticAnalysis::LeaveFunction() {
        base::ScopedPtr<LexicalFunction> function = current_function_;
        current_function_ = function->parent_;
    }
    
    void SemanticAnalysis::EnterBlock() {
        LexicalBlock* block = new LexicalBlock;
        block->parent_ = current_function_->current_block_;
        current_function_->current_block_.Reset(block);
    }
    
    void SemanticAnalysis::LeaveBlock() {
        base::ScopedPtr<LexicalBlock> block = current_function_->current_block_;
        current_function_->current_block_ = block->parent_;
    }
    
    bool SemanticAnalysis::InsertName(const String* name) {
        LexicalBlock* block = current_function_->current_block_.Get();
        if(block->names_.find(const_cast<String*>(name)) != block->names_.end()) {
            return false;
        }
        block->names_.insert(const_cast<String*>(name));
        return true;
    }
    
    LexicalScoping SemanticAnalysis::SearchName(const String* name) {
        LexicalFunction* function = current_function_.Get();
        while(function) {
            LexicalBlock* block = function->current_block_.Get();
            while(block) {
                std::unordered_set<String*>::iterator iter = block->names_.find(const_cast<String*>(name));
                if(iter != block->names_.end()) {
                    return function == current_function_.Get() ? LexicalScoping_Local : LexicalScoping_Upvalue;
                }
                block = block->parent_.Get();
            }
            function = function->parent_.Get();
        }
        return LexicalScoping_Global;
    }
    
    void SemanticAnalysis::Visit(ChunkAST* ast, void* data) {
        Guard<SemanticAnalysis> g(this, &SemanticAnalysis::EnterFunction, &SemanticAnalysis::LeaveFunction);
        {
            Guard<SemanticAnalysis> g(this, &SemanticAnalysis::EnterBlock, &SemanticAnalysis::LeaveBlock);
            ast->block()->Accept(this, nullptr);
        }
    }
    
    void SemanticAnalysis::Visit(BlockAST* ast, void* data) {
        for(base::ScopedVector<ASTree>::iterator iter = ast->statements().begin();
            iter != ast->statements().end(); ++iter) {
            (*iter)->Accept(this, nullptr);
        }
        if(ast->return_statement().Get()) {
            ast->return_statement()->Accept(this, nullptr);
        }
    }
    
    void SemanticAnalysis::Visit(ReturnStatementAST* ast, void* data){
        ExprData expr_data;
        expr_data.lex_po_ = LexicalOp_Read;
        if(ast->expression().Get())
            ast->expression()->Accept(this, &expr_data);
    }
    
    void SemanticAnalysis::Visit(LiteralAST* ast, void* data){
        ExprData* expr_data = static_cast<ExprData*>(data);
        switch (ast->token().token_) {
            case Token_Nil:
                if(expr_data)
                    expr_data->expr_type_ = ExprType_Nil;
                break;
            case Token_Id:
                if(expr_data)
                    expr_data->expr_type_ = ExprType_Unknown;
                break;
            case Token_Number:
                if(expr_data)
                    expr_data->expr_type_ = ExprType_Number;
                break;
            case Token_String:
                if(expr_data)
                    expr_data->expr_type_ = ExprType_String;
                break;
            case Token_False:
            case Token_True:
                if(expr_data)
                    expr_data->expr_type_ = ExprType_Bool;
                break;
            default:
                break;
        }
        if(ast->token().token_ == Token_Id) {
            ast->scope() = SearchName(ast->token().str_);
        }
        ast->lex_op() = expr_data == nullptr ? LexicalOp_Read : expr_data->lex_po_;
    }
    
    void SemanticAnalysis::Visit(NamesAST* ast, void* data){
        for(std::vector<Token>::iterator iter = ast->names().begin();
            iter != ast->names().end(); ++iter) {
            if(!InsertName((*iter).str_)) {
                throw CompileException((*iter).str_->c_str(), " is already existed", (*iter));
            }
        }
    }
    
    void SemanticAnalysis::Visit(BinaryExprAST* ast, void* data){
        ExprData l_expr_data, r_expr_data;
        l_expr_data.lex_po_ = LexicalOp_Read;
        r_expr_data.lex_po_ = LexicalOp_Read;
        if(ast->left().Get() == NULL) {
            throw CompileException("something wrong with operator", ast->op_token());
        }
        ast->left()->Accept(this, &l_expr_data);
        if(ast->right().Get() == NULL) {
            throw CompileException("something wrong with operator", ast->op_token());
        }
        ast->right()->Accept(this, &r_expr_data);
        
        ExprData* expr_data = static_cast<ExprData*>(data);
        
        switch (ast->op_token().token_) {
            case '+':
                // 这里有可能有数字与文字的相加，所以只要是Number和String就可以
                if(!(l_expr_data.expr_type_ == ExprType_Number ||
                     l_expr_data.expr_type_ == ExprType_String)) {
                    throw CompileException("left expression is not number or string", ast->op_token());
                }
                if(!(r_expr_data.expr_type_ == ExprType_Number ||
                     r_expr_data.expr_type_ == ExprType_String)) {
                    throw CompileException("right expression is not number or string", ast->op_token());
                }
                if(r_expr_data.expr_type_ == ExprType_String || l_expr_data.expr_type_ == ExprType_String){
                    expr_data->expr_type_ = ExprType_String;
                }else{
                    expr_data->expr_type_ = ExprType_Number;
                }
                break;
            case '-':
            case '*':
            case '/':
            case '%':
            case '^':
                if(!(l_expr_data.expr_type_ == ExprType_Number ||
                   l_expr_data.expr_type_ == ExprType_Unknown)) {
                    throw CompileException("left expression is not number", ast->op_token());
                }
                
                if(!(r_expr_data.expr_type_ == ExprType_Number ||
                   r_expr_data.expr_type_ == ExprType_Unknown)) {
                    throw CompileException("right expression is not number", ast->op_token());
                }
                
                expr_data->expr_type_ = ExprType_Number;
                break;
            case '<':
            case '>':
            case Token_LessEqual:
            case Token_GreaterEqual:
                if(l_expr_data.expr_type_ != r_expr_data.expr_type_ &&
                   !(l_expr_data.expr_type_ == ExprType_Unknown ||
                   r_expr_data.expr_type_ == ExprType_Unknown)) {
                    throw CompileException("compare different expression type", ast->op_token());
                }
                 expr_data->expr_type_ = ExprType_Bool;
                break;
            case Token_NotEqual:
            case Token_Equal:
                expr_data->expr_type_ = ExprType_Bool;
            default:
                break;
        }
        
    }
    
    void SemanticAnalysis::Visit(UnaryExpression* ast, void* data){
        ExprData unary_expr_data;
        unary_expr_data.lex_po_ = LexicalOp_Read;
        ast->expression()->Accept(this, &unary_expr_data);
        if(unary_expr_data.expr_type_ != ExprType_Unknown) {
            switch (ast->op_token().token_) {
                case '-':
                    if(unary_expr_data.expr_type_ != ExprType_Number) {
                        throw CompileException("operand is not number", ast->op_token());
                    }
                    break;
                default:
                    break;
            }
        }
        
        ExprData* expr_data = static_cast<ExprData*>(data);
        if(ast->op_token().token_ == '-') {
            expr_data->expr_type_ = ExprType_Number;
        }else if(ast->op_token().token_ == Token_Not) {
            expr_data->expr_type_ = ExprType_Bool;
        }
    }
    
    void SemanticAnalysis::Visit(ExpressionListAST* ast, void* data){
        for(base::ScopedVector<ASTree>::iterator iter = ast->expressions().begin();
            iter != ast->expressions().end(); ++iter) {
            if(*iter == NULL)
                continue;
            ExprData expr_data;
            expr_data.lex_po_ = LexicalOp_Read;
            (*iter)->Accept(this, &expr_data);
        }
    }
    
    void SemanticAnalysis::Visit(VariableAST* ast, void* data){
        ExprData expr_data;
        expr_data.lex_po_ = LexicalOp_Read;
        ast->expression()->Accept(this, &expr_data);
        if(ast->identifier().token_ != Token_Id) {
            //TODO exception
        }
        if(!InsertName(ast->identifier().str_)) {
            throw CompileException(ast->identifier().str_->c_str(), " is already existed", ast->identifier());
        }
    }
    
    void SemanticAnalysis::Visit(VariableListAST* ast, void* data){
        for(base::ScopedVector<VariableAST>::iterator iter = ast->variable_list().begin();
            iter != ast->variable_list().end(); ++iter) {
            (*iter)->Accept(this, nullptr);
        }
    }
    
    void SemanticAnalysis::Visit(FunctionStatementAST* ast, void* data){
        if(!InsertName(ast->function_name().str_)) {
            throw CompileException(ast->function_name().str_->c_str(), " is already existed", ast->function_name());
        }
        Guard<SemanticAnalysis> g(this, &SemanticAnalysis::EnterFunction, &SemanticAnalysis::LeaveFunction);
        {
            Guard<SemanticAnalysis> g(this, &SemanticAnalysis::EnterBlock, &SemanticAnalysis::LeaveBlock);
            ast->params()->Accept(this, nullptr);
            ast->body()->Accept(this, nullptr);
        }
    }
    
    void SemanticAnalysis::Visit(ForStatementAST* ast, void* data) {
        Guard<SemanticAnalysis> g(this, &SemanticAnalysis::EnterBlock, &SemanticAnalysis::LeaveBlock);
        ast->statement1()->Accept(this, nullptr);
        ExprData expr_data;
        expr_data.lex_po_ = LexicalOp_Read;
        ast->statement2()->Accept(this, &expr_data);
        for(base::ScopedVector<ASTree>::iterator iter = ast->statement3().begin();
            iter != ast->statement3().end(); ++iter) {
            //TODO check statements
            (*iter)->Accept(this, nullptr);
        }
        ast->block()->Accept(this, nullptr);
    }
    
    void SemanticAnalysis::Visit(DoWhileStatementAST* ast, void* data){
        ExprData expr_data;
        expr_data.lex_po_ = LexicalOp_Read;
        ast->condition()->Accept(this, &expr_data);
        {
            Guard<SemanticAnalysis> g(this, &SemanticAnalysis::EnterBlock, &SemanticAnalysis::LeaveBlock);
            ast->block()->Accept(this, nullptr);
        }
    }
    
    void SemanticAnalysis::Visit(BreakStatementAST* ast, void* data){
        
    }
    
    void SemanticAnalysis::Visit(WhileStatementAST* ast, void* data){
        ExprData expr_data;
        expr_data.lex_po_ = LexicalOp_Read;
        ast->condition()->Accept(this, &expr_data);
        {
            Guard<SemanticAnalysis> g(this, &SemanticAnalysis::EnterBlock, &SemanticAnalysis::LeaveBlock);
            ast->block()->Accept(this, nullptr);
        }
    }
    
    void SemanticAnalysis::Visit(IfStatementAST* ast, void* data){
        ExprData expr_data;
        expr_data.lex_po_ = LexicalOp_Read;
        ast->condition()->Accept(this, &expr_data);
        {
            Guard<SemanticAnalysis> g(this, &SemanticAnalysis::EnterBlock, &SemanticAnalysis::LeaveBlock);
            ast->true_branch()->Accept(this, nullptr);
        }
        
        if(ast->false_branch().Get()) {
            ast->false_branch()->Accept(this, nullptr);
        }
    }
    
    void SemanticAnalysis::Visit(ElseStatementAST* ast, void* data){
        Guard<SemanticAnalysis> g(this, &SemanticAnalysis::EnterBlock, &SemanticAnalysis::LeaveBlock);
        ast->block()->Accept(this, nullptr);
    }
    
    void SemanticAnalysis::Visit(SwitchStatementAST* ast, void* data) {
        ExprData expr_data;
        expr_data.lex_po_ = LexicalOp_Read;
        ast->expression()->Accept(this, &expr_data);
        int last_token = -1;
        for(base::ScopedVector<ASTree>::iterator iter = ast->cases().begin();
            iter != ast->cases().end(); ++iter) {
            Guard<SemanticAnalysis> g(this, &SemanticAnalysis::EnterBlock, &SemanticAnalysis::LeaveBlock);
            int token = static_cast<CaseStatementAST*>(*iter)->key().token_;
            if(last_token != -1 && last_token != token) {
                throw CompileException("case type error ", static_cast<CaseStatementAST*>(*iter)->key());
            }
            (*iter)->Accept(this, nullptr);
        }
    }
    
    void SemanticAnalysis::Visit(CaseStatementAST* ast, void* data) {
        ast->block()->Accept(this, nullptr);
    }
    
    void SemanticAnalysis::Visit(AssignStatement* ast, void* data){
        ExprData expr_data;
        switch (ast->assignment().token_) {
            case '=':
                expr_data.lex_po_ = LexicalOp_Write;
                break;
            case Token_ASSIGN_ADD:
                expr_data.lex_po_ = LexicalOp_ASSIGN_ADD;
                break;
            case Token_ASSIGN_SUB:
                expr_data.lex_po_ = LexicalOp_ASSIGN_SUB;
                break;
            case Token_ASSIGN_MUL:
                expr_data.lex_po_ = LexicalOp_ASSIGN_MUL;
                break;
            case Token_ASSIGN_DIV:
                expr_data.lex_po_ = LexicalOp_ASSIGN_DIV;
                break;
            case Token_ASSIGN_MOD:
                expr_data.lex_po_ = LexicalOp_ASSIGN_MOD;
                break;
            default:
                break;
        }
        ast->variable()->Accept(this, &expr_data);
        expr_data.lex_po_ = LexicalOp_Read;
        ast->expression()->Accept(this, &expr_data);
    }
    
    void SemanticAnalysis::Visit(MemberAccessorAST* ast, void* data){
        ast->table()->Accept(this, data);
    }
    
    void SemanticAnalysis::Visit(FunctionCallAST* ast, void* data) {
        ExprData expr_data;
        expr_data.lex_po_ = LexicalOp_Read;
        ast->caller()->Accept(this, &expr_data);
        ast->args()->Accept(this, nullptr);
    }
}
