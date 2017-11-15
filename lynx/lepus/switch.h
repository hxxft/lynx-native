
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
        
        void Modify(Token& key, int offset);
        void Adjust();
        int Switch(Value* value);
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
