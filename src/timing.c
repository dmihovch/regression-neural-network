#include "../include/timing.h"

#ifdef __APPLE__
#include <mach/mac_time.h>
double get_time_sec(){
    static mach_timebase_info_data_t time;
    if(time.denom == 0){
        mach_timebase_info(&time);
    }
    return mach_absolute_time() * (1e-9 * time.number / time.denom);
}
#else
#include <time.h>
double get_time_sec(){
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec + time.tv_nsec * 1e-9;
}
#endif
