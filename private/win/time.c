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

System_Time mimas_platform_get_utc_system_time(void) {
    SYSTEMTIME t = {};
    GetSystemTime(&t);
    return {t.wYear, t.wMonth, t.wDay, t.wDayOfWeek, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds};
}

System_Time mimas_platform_get_local_system_time(void) {
    SYSTEMTIME t = {};
    GetLocalTime(&t);
    return {t.wYear, t.wMonth, t.wDay, t.wDayOfWeek, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds};
}
