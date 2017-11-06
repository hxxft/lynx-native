
#ifndef LYNX_LEPUS_GUARD_H_
#define LYNX_LEPUS_GUARD_H_

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
}

#endif
