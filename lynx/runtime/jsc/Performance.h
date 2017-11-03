//
// Created by dli on 9/19/16.
//

#ifndef JSCORE_PERFORMANCE_H
#define JSCORE_PERFORMANCE_H

#include<sys/time.h>

namespace jscore {

    class Performance {
    public:
        Performance(const char *log);

        Performance(const char *log, bool use_micro_second);

        ~Performance();

    private:
        timeval time_;
        const char *log_;
        bool use_micro_second_;
    };
}


#endif //JSCORE_PERFORMANCE_H
