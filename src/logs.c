
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

#include "constants.h"
#include "custom_types.h"
#include "utils.h"


#include "logs.h"

void set_log_level(log_level_t level)
{
    log_level = level;
}

void log_message(log_level_t level, const char *message, ...)
{
    // skip logging if the level is lower than the set log_level
    if ((level < log_level) || (level == LOG_NONE))
    {
        return;
    }

    va_list args;
    va_start(args, message);

    // Get current time
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char time_fmt[20];
    strftime(time_fmt, sizeof(time_fmt), "%Y-%m-%d %H:%M:%S", tm_info);

    const char *level_str;
    switch (level)
    {
    case LOG_DEBUG:
        level_str = "DEBUG";
        break;
    case LOG_INFO:
        level_str = "INFO";
        break;
    case LOG_WARNING:
        level_str = "WARNING";
        break;
    case LOG_ERROR:
        level_str = "ERROR";
        break;
    default:
        level_str = "UNKNOWN";
        break;
    }

    char log_prefix[LOG_PREFIX_LEN];
    snprintf(log_prefix, sizeof(log_prefix), "[%s]", level_str);
    int crn_len = strlen(log_prefix);
    if (crn_len < LOG_PREFIX_LEN)
    {
        memset(log_prefix + crn_len, ' ', LOG_PREFIX_LEN - crn_len);
        log_prefix[LOG_PREFIX_LEN - 1] = '\0';
    }

    fprintf(stderr, "%s - (%s):\t", log_prefix, time_fmt);
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");

    va_end(args);
}