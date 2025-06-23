#ifndef STATS_H
#define STATS_H

#include <stdint.h>
#include <stdio.h>

struct stats
{
    uint32_t points;
    uint16_t level;
    uint16_t lines_cleared;
    uint64_t time;
};

void stats_time_as_str(char str[16], const uint64_t time_ms);

#endif
