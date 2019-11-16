#ifndef TIMING_UTIL_HPP
#define TIMING_UTIL_HPP

#include <chrono>
#include <cstdint>

namespace osrm{
    namespace util{
        // 用TIMER_START 定义一个变量记录开始的时间
        #define TIMER_START(_X) auto _X##_start = std::chrono::steady_clock::now(), _X##_stop = _X##_start
        // 用TIMER_STOP 定义一个变量记录结束的时间
        #define TIMER_STOP(_X) _X##_stop = std::chrono::steady_clock::now()
        // TIMER_NSEC 定义start到stop经历了多少纳秒 
        #define TIMER_NSEC(_X)                                                                             \
            std::chrono::duration_cast<std::chrono::nanoseconds>(_X##_stop - _X##_start).count()
        // TIMER_USEC 定义start到stop历经多少微秒
        #define TIMER_USEC(_X)                                                                             \
            std::chrono::duration_cast<std::chrono::microseconds>(_X##_stop - _X##_start).count()
        // TIMER_MSEC 定义start到stop经历多少毫秒
        #define TIMER_MSEC(_X)                                                                             \
            (0.000001 *                                                                                    \
            std::chrono::duration_cast<std::chrono::nanoseconds>(_X##_stop - _X##_start).count())
        // TIMER_SEC 定义start到stop经历多少秒
        #define TIMER_SEC(_X)                                                                              \
            (0.000001 *                                                                                    \
            std::chrono::duration_cast<std::chrono::microseconds>(_X##_stop - _X##_start).count())
        // TIMER_MIN 定义start到stop经历多少分钟
        #define TIMER_MIN(_X)                                                                              \
            std::chrono::duration_cast<std::chrono::minutes>(_X##_stop - _X##_start).count()
        //统计耗时（ms）
        #define TIME_COST_MS(name) std::cout << name <<" Cost: " << TIMER_MSEC(x) << " ms.\n"
        //统计耗时（s）
        #define TIME_COST_S(name) std::cout <<name<< " Cost :" << TIMER_SEC(x) << " s.\n"
        //统计耗时（min）
        #define TIME_COST_MIN(name) std::cout <<name<< " Cost: " << TIMER_SEC(x) << " s.\n"
    }
}

#endif // TIMING_UTIL_HPP