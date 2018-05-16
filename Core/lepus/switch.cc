#include <algorithm>
#include "lepus/switch.h"
#include "lepus/value.h"

namespace lepus {
    SwitchInfo* SwitchInfo::Create(base::ScopedVector<ASTree>& cases) {
        SwitchInfo* info = lynx_new SwitchInfo;
        CaseStatementAST* ast = static_cast<CaseStatementAST*>(cases[0]);
        if(ast->key().token_ == Token_Number) {
            info->key_type_ = Token_Number;
        }else if(ast->key().token_ == Token_String) {
            info->key_type_ = Token_String;
        }
        
        std::vector<std::pair<long, long> > table;
        for(base::ScopedVector<ASTree>::iterator iter = cases.begin();
            iter != cases.end(); ++iter) {
            CaseStatementAST* ast = static_cast<CaseStatementAST*>(*iter);
            if(ast->is_default()){
                continue;
            }
            if(info->key_type_ == Token_Number) {
                info->min_ = info->min_ < ast->key().number_ ? info->min_ : ast->key().number_;
                info->max_ = info->max_ < ast->key().number_ ? ast->key().number_: info->max_;
                table.push_back(std::make_pair(ast->key().number_, -1));
            }else if(info->key_type_ == Token_String){
                table.push_back(std::make_pair(ast->key().str_->hash(), -1));
            }
               
        }
        double useful_rate = cases.size() / (info->max_ - info->min_);
        
        if(useful_rate > 0.7) {
            long size = info->max_ - info->min_ + 1;
            info->switch_table_.resize(size);
            for(base::ScopedVector<ASTree>::iterator iter = cases.begin();
                iter != cases.end(); ++iter) {
                info->switch_table_[(int)ast->key().number_] = std::make_pair(ast->key().number_, -1);
            }
            info->type_ = SwitchType_Table;
        }else{
            std::swap(info->switch_table_, table);
            info->type_ = SwitchType_Lookup;
            info->Adjust();
        }
        return info;
    }
    
    // 升序排列
    bool SwitchInfo::SortTable(const std::pair<int, int> &v1, const std::pair<int, int> &v2 ){
        return v1.first < v2.first;
    }
    
    long SwitchInfo::BinarySearchTable(long key){
        if(switch_table_.size() == 0){
            return -1;
        }
        long low = 0;
        long high = switch_table_.size() - 1;
        long mid = 0;
        while(low <= high){
            mid = (low + high) / 2;
            if(switch_table_[mid].first < key){
                low = mid + 1;
            }else if(switch_table_[mid].first > key){
                high = mid-1;
            }else{
                return mid;
            }
        }
        return -1;
    }
    
    
    void SwitchInfo::Modify(Token& key, long offset) {
        if(key.token_ == Token_Defalut) {
            default_offset_ = offset;
            return;
        }
        if(type_ == SwitchType_Table) {
            if(key.token_ != Token_Number || min_ > max_) {
                return ;
            }
            
            long k = (long)key.number_;
            if(k < min_ || k > max_) {
                default_offset_ = offset;
                return;
            }
            long index = k - min_;
            
            switch_table_[index].second = offset;
            return;
        }else if(type_ == SwitchType_Lookup){
            long key_index;
            if(key.token_ == Token_Number) {
                key_index = key.number_;
            }else if(key.token_ == Token_String){
                key_index = key.str_->hash();
            }else{
                return;
            }
            long search_index = BinarySearchTable(key_index);
            if(search_index != -1){
                switch_table_[search_index].second = offset;
            }
        }
    }
    
    void SwitchInfo::Adjust() {
        if(type_ != SwitchType_Table) {
            std::sort(switch_table_.begin(), switch_table_.end(), SortTable);
        }
    }
    
    long SwitchInfo::Switch(Value* value) {
        if(type_ == SwitchType_Table) {
            if(value->type_ != Value_Number || min_ > max_) {
                return -1;
            }
            long v = (long)value->number_;
            long index = v - min_;
            if(v < min_ || v > max_ || switch_table_[index].second < 0) {
                return default_offset_;
            }
            return switch_table_[index].second;
        }else if(type_ == SwitchType_Lookup){
            long key_index = 0;
            if(value->type_ == Value_Number) {
                key_index = value->number_;
            }else if(value->type_ == Value_String){
                key_index = value->str_->hash();
            }
            long search_index = BinarySearchTable(key_index);
            if(search_index != -1){
                return switch_table_[search_index].second;
            }
            return default_offset_;
        }
        return -1;
    }
}
