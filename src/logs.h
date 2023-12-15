#ifndef LOGS_H
#define LOGS_H
#include "custom_types.h"

/**
 * @brief Sets the global log level.
 *
 * The log level determines the severity of messages that will be logged.
 * Messages with a log level lower than the set level will not be logged.
 *
 * @param level The desired log level. Must be one of the log_level_t enum values.
 *
 * @example
 *     set_log_level(LOG_ERROR); // Only error messages will be logged
 */
void set_log_level(log_level_t level);

/**
 * @brief This function writes a log message to the standard error stream (stderr).
 * Messages can be filtered by setting the global log level. If the specified
 * log level is lower than the current global log level, the message will
 * not be logged.
 *
 * The message can include format specifiers, similar to printf. The log
 * message is prefixed with a log level and the timestamp.
 *
 * @param level The log level for this message. Must be one of the log_level_t
 *              enum values.
 * @param message The format string for the log message (printf-style).
 * @param ... Additional arguments for format specifiers in the message.
 *
 * @example
 *     log_message(LOG_INFO, "This is an %s message with number %d", "info", 42);
 */
void log_message(log_level_t level, const char *message, ...);

#endif // LOGS_H
