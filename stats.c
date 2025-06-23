#include "stats.h"

void stats_time_as_str(char str[16], const uint64_t time_ms)
{
    uint32_t seconds = time_ms / 1000;
    uint32_t minutes = seconds / 60;
    uint32_t hours = minutes / 60;

    char seconds_str[16];
    sprintf(seconds_str, seconds % 60 >= 10 ? "%d" : "0%d", seconds % 60);

    char minutes_str[16];
    sprintf(minutes_str, minutes % 60 >= 10 ? "%d" : "0%d", minutes % 60);

    char hours_str[16];
    sprintf(hours_str, hours % 24 >= 10 ? "%d" : "0%d", hours % 24);

    if (hours > 0)
    {
        sprintf(str, "%s:%s:%s", hours_str, minutes_str, seconds_str);
    }
    else
    {
        sprintf(str, "%s:%s", minutes_str, seconds_str);
    }
}

