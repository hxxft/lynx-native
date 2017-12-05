
#ifndef LYNX_LEPUS_SWITCH_H_
#define LYNX_LEPUS_SWITCH_H_

#include <vector>
#include "lepus/syntax_tree.h"
#include "lepus/token.h"

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
             min_(INT_MAX),
             max_(0),
             default_offset_(-1),
             switch_table_(){}
        
        void Modify(Token& key, int offset);
        int BinarySearchTable(int key);
        static bool SortTable(const std::pair<int, int> &v1, const std::pair<int, int> &v2 );
        void Adjust();
        int Switch(Value* value);
        int default_offset() {
            return default_offset_;
        }
        
        void set_default_offset(int offset) {
            default_offset_ = offset;
        }
    private:
        SwitchType type_;
        int key_type_;
        int min_;
        int max_;
        int default_offset_;
        std::vector<std::pair<int, int> > switch_table_;
    };
}

#endif
