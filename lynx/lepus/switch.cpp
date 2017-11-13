#include "lepus/switch.h"

namespace lepus {
    SwitchInfo* SwitchInfo::Create(base::ScopedVector<ASTree>& cases) {
        SwitchInfo* info = new SwitchInfo;
        CaseStatementAST* ast = static_cast<CaseStatementAST*>(cases[0]);
        if(ast->key().token_ == Token_Number) {
            info->key_type_ = Token_Number;
        }else if(ast->key().token_ == Token_String) {
            info->key_type_ = Token_String;
        }
        
        std::vector<std::pair<int, int> > table;
        for(base::ScopedVector<ASTree>::iterator iter = cases.begin();
            iter != cases.end(); ++iter) {
            CaseStatementAST* ast = static_cast<CaseStatementAST*>(*iter);
            if(info->key_type_ == Token_Number) {
                info->min_ = info->min_ < ast->key().number_ ? info->min_ : ast->key().number_;
                info->max_ = info->max_ < ast->key().number_ ? ast->key().number_: info->max_;
                table.push_back(std::make_pair(ast->key().number_, -1));
            }else{
                //table.push_back(std::make_pair(ast->key().number_, -1));
            }
        }
        
        double useful_rate = cases.size() / info->max_ - info->min_;
        if(useful_rate > 0.7) {
            int size = info->max_ - info->min_ + 1;
            info->switch_table_.resize(size);
            for(base::ScopedVector<ASTree>::iterator iter = cases.begin();
                iter != cases.end(); ++iter) {
                info->switch_table_[(int)ast->key().number_] = std::make_pair(ast->key().number_, -1);
            }
            info->type_ = SwitchType_Table;
        }else{
            info->switch_table_ = table;
            info->type_ = SwitchType_Lookup;
        }
        return info;
    }
    
    void SwitchInfo::Modify(Token& key, int offset) {
        if(type_ == SwitchType_Table) {
            if(key.token_ != Token_Number || min_ > max_) {
                return ;
            }
            int k = (int)key.number_;
            int index = k - min_;
            if(k < min_ || k > max_) {
                default_offset_ = offset;
            }
            switch_table_[index].second = offset;
            return;
        }else {
            
        }
    }
    
    void SwitchInfo::Adjust() {
        if(type_ != SwitchType_Table) {
            
        }
    }
    
    int SwitchInfo::Switch(Value* value) {
        if(type_ == SwitchType_Table) {
            if(value->type_ != Value_Number || min_ > max_) {
                return -1;
            }
            int v = (int)value->number_;
            int index = v - min_;
            if(v < min_ || v > max_ || switch_table_[index].second < 0) {
                return default_offset_;
            }
            return switch_table_[index].second;
        }else {
            
        }
    }
}
