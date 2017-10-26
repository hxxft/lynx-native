
#include "lepus/semantic_analysis.h"

#include "lepus/exception.h"

namespace lepus {
    
    template<class Class>
    class Guard {
    public:
        typedef void (Class::*CallbackFunc)();
        Guard(Class* ptr, CallbackFunc enter, CallbackFunc leave)
        :ptr_(ptr), enter_(enter), leave_(leave){
            if(ptr_ && enter_)
                (ptr_->*enter_)();
        }
        
        Guard() {
            if(ptr_ && enter_)
                (ptr_->*leave_)();
        }
    private:
        Class* ptr_;
        CallbackFunc enter_;
        CallbackFunc leave_;
    };
    
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
        ExprData():expr_type_(ExprType_Unknown){}
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
    
    void SemanticAnalysis::InsertName(const std::string& name) {
        current_function_->current_block_->names_.insert(name);
    }
    
    LexicalScoping SemanticAnalysis::SearchName(const std::string& name) {
        LexicalFunction* function = current_function_.Get();
        while(function) {
            LexicalBlock* block = function->current_block_.Get();
            while(block) {
                std::unordered_set<std::string>::iterator iter = block->names_.find(name);
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
        
    }
    
    void SemanticAnalysis::Visit(LiteralAST* ast, void* data){
        ExprData* expr_data = static_cast<ExprData*>(data);
        switch (ast->token().token_) {
            case Token_Nil:
                expr_data->expr_type_ = ExprType_Nil;
                break;
            case Token_Id:
                expr_data->expr_type_ = ExprType_Unknown;
                break;
            case Token_Number:
                expr_data->expr_type_ = ExprType_Number;
                break;
            case Token_String:
                expr_data->expr_type_ = ExprType_String;
                break;
            case Token_False:
            case Token_True:
                expr_data->expr_type_ = ExprType_Bool;
                break;
            default:
                break;
        }
        if(ast->token().token_ == Token_Id) {
            ast->scope() = SearchName(*ast->token().str_);
        }
    }
    
    void SemanticAnalysis::Visit(NamesAST* ast, void* data){
        for(std::vector<Token>::iterator iter = ast->names().begin();
            iter != ast->names().end(); ++iter) {
            InsertName(*(*iter).str_);
        }
    }
    
    void SemanticAnalysis::Visit(BinaryExprAST* ast, void* data){
        ExprData l_expr_data, r_expr_data;
        ast->left()->Accept(this, &l_expr_data);
        ast->right()->Accept(this, &r_expr_data);
        
        ExprData* expr_data = static_cast<ExprData*>(data);
        
        switch (ast->op_token().token_) {
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '^':
                if(!(l_expr_data.expr_type_ == ExprType_Number ||
                   l_expr_data.expr_type_ == ExprType_Unknown)) {
                    throw SemantException("left expression is not number", ast->op_token());
                }
                
                if(!(r_expr_data.expr_type_ == ExprType_Number ||
                   r_expr_data.expr_type_ == ExprType_Unknown)) {
                    throw SemantException("right expression is not number", ast->op_token());
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
                    throw SemantException("compare different expression type", ast->op_token());
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
        ast->expression()->Accept(this, &unary_expr_data);
        if(unary_expr_data.expr_type_ != ExprType_Unknown) {
            switch (ast->op_token().token_) {
                case '-':
                    if(unary_expr_data.expr_type_ != ExprType_Number) {
                        throw SemantException("operand is not number", ast->op_token());
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
            ExprData expr_data;
            (*iter)->Accept(this, &expr_data);
        }
    }
    
    void SemanticAnalysis::Visit(VariableAST* ast, void* data){
        ExprData expr_data;
        ast->expression()->Accept(this, &expr_data);
        if(ast->identifier().token_ != Token_Id) {
            //TODO exception
        }
        InsertName(*(ast->identifier().str_));
    }
    
    void SemanticAnalysis::Visit(VariableListAST* ast, void* data){
        for(base::ScopedVector<VariableAST>::iterator iter = ast->variable_list().begin();
            iter != ast->variable_list().end(); ++iter) {
            (*iter)->Accept(this, nullptr);
        }
    }
    
    void SemanticAnalysis::Visit(FunctionStatementAST* ast, void* data){
        InsertName(*(ast->function_name().str_));
        Guard<SemanticAnalysis> g(this, &SemanticAnalysis::EnterFunction, &SemanticAnalysis::LeaveFunction);
        {
            Guard<SemanticAnalysis> g(this, &SemanticAnalysis::EnterBlock, &SemanticAnalysis::LeaveBlock);
            ast->params()->Accept(this, nullptr);
            ast->body()->Accept(this, nullptr);
        }
    }
    
    void SemanticAnalysis::Visit(BreakStatementAST* ast, void* data){
        
    }
    
    void SemanticAnalysis::Visit(WhileStatementAST* ast, void* data){
        
    }
    
    void SemanticAnalysis::Visit(IfStatementAST* ast, void* data){
        ExprData expr_data;
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
    
    void SemanticAnalysis::Visit(AssignStatement* ast, void* data){
        ExprData expr_data;
        ast->variable()->Accept(this, nullptr);
        ast->expression()->Accept(this, &expr_data);
    }
    
    void SemanticAnalysis::Visit(MemberAccessorAST* ast, void* data){
        
    }
    
    void SemanticAnalysis::Visit(FunctionCallAST* ast, void* data) {
        ExprData expr_data;
        ast->caller()->Accept(this, &expr_data);
        ast->args()->Accept(this, nullptr);
    }
}
