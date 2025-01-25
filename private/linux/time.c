#include <mimas/mimas.h>
#include <platform.h>

#include <time.h>

Mimas_System_Time mimas_platform_get_utc_system_time(void)
{
  time_t tm = time(NULL);
  struct tm t = {0};
  gmtime(&tm);
  return (Mimas_System_Time){.year = t.tm_year + 1900,
                             .month = t.tm_mon + 1,
                             .day = t.tm_mday,
                             .day_of_week = t.tm_wday,
                             .hour = t.tm_hour,
                             .minutes = t.tm_min,
                             .seconds = t.tm_sec,
                             .milliseconds = 0};
}

Mimas_System_Time mimas_platform_get_local_system_time(void)
{
  time_t tm = time(NULL);
  struct tm t = {0};
  localtime(&tm);
  return (Mimas_System_Time){.year = t.tm_year,
                             .month = t.tm_mon,
                             .day = t.tm_mday,
                             .day_of_week = t.tm_wday,
                             .hour = t.tm_hour,
                             .minutes = t.tm_min,
                             .seconds = t.tm_sec,
                             .milliseconds = 0};
}

double mimas_platform_get_time(void)
{
  struct timespec ts = {0};
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
  return (double)ts.tv_sec + (double)ts.tv_nsec * 0.000000001;
}
