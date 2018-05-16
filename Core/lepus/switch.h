
#ifndef LYNX_LEPUS_SWITCH_H_
#define LYNX_LEPUS_SWITCH_H_

#include <vector>
#include "lepus/syntax_tree.h"
#include "lepus/token.h"
#include <algorithm>

namespace lepus {
    enum SwitchType {
        SwitchType_Table,
        SwitchType_Lookup,
    };
    
    class Value;
    class SwitchInfo {
    public:
        static SwitchInfo* Create(base::ScopedVector<ASTree>& cases);
        
        SwitchInfo()
            :key_type_(0),
             min_(LONG_MAX),
             max_(0),
             default_offset_(-1),
             switch_table_(){}
        
        void Modify(Token& key, long offset);
        long BinarySearchTable(long key);
        static bool SortTable(const std::pair<int, int> &v1, const std::pair<int, int> &v2 );
        void Adjust();
        long Switch(Value* value);
        long default_offset() {
            return default_offset_;
        }
        
        void set_default_offset(long offset) {
            default_offset_ = offset;
        }
    private:
        SwitchType type_;
        long key_type_;
        long min_;
        long max_;
        long default_offset_;
        std::vector<std::pair<long, long> > switch_table_;
    };
}

#endif
