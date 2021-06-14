#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>
#include <iostream>
#include <iomanip>

class Timer
{
private:
    std::chrono::high_resolution_clock::time_point start;
public:
    Timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    friend std::ostream& operator<<(std::ostream& os, const Timer& t)
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - t.start);
        os << '['  << duration.count() / 1000.0 << " s]";
        return os;
    }

    int64_t get_time()
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        return duration.count();
    }
};

#endif