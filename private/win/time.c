#include <platform.h>
#include <win/platform.h>
#include <mimas/mimas.h>

static u64 get_performance_counter(void) {
    u64 counts;
    // The output value is always nonzero and we may safely ignore the return value since:
    //   "On systems that run Windows XP or later, the function will always succeed and will thus never return zero."
    //   https://docs.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancecounter
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&counts));
    return counts;
}

static u64 get_win_performance_frequency(void) {
    u64 frequency;
    // The output value is always nonzeroand we may safely ignore the return value since:
    //   "On systems that run Windows XP or later, the function will always succeed and will thus never return zero."
    //   https://docs.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancefrequency
    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
    return frequency;
}

static u64 get_performance_frequency(void) {
    static u64 frequency = get_win_performance_frequency();
    return frequency;
}

double mimas_platform_get_time(void) {
    return static_cast<double>(get_performance_counter()) / static_cast<double>(get_performance_frequency());
}

Mimas_System_Time mimas_platform_get_utc_system_time(void) {
    SYSTEMTIME t = {};
    GetSystemTime(&t);
    return (Mimas_System_Time){ .year = t.wYear, 
                                .month = t.wMonth, 
                                .day = t.wDay, 
                                .day_of_week = t.wDayOfWeek, 
                                .hour = t.wHour, 
                                .minutes = t.wMinute, 
                                .seconds = t.wSecond, 
                                .milliseconds = t.wMilliseconds };
}

Mimas_System_Time mimas_platform_get_local_system_time(void) {
    SYSTEMTIME t = {};
    GetLocalTime(&t);
    return (Mimas_System_Time){ .year = t.wYear, 
                                .month = t.wMonth, 
                                .day = t.wDay, 
                                .day_of_week = t.wDayOfWeek, 
                                .hour = t.wHour, 
                                .minutes = t.wMinute, 
                                .seconds = t.wSecond, 
                                .milliseconds = t.wMilliseconds };
}
